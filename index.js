/**
 * Module dependencies.
 */
const authKeys = ['dGVzdDp0ZXN0'] //Base64('test:test'), replace with environment variable and valid credentials.

const mongoose = require('mongoose')
const express = require('express')
const bodyParser = require('body-parser')
const { Sensor, Reading } = require('./models')

const { logger, errorLogger } = require('./utils/logger')
const startApp = async () => {
  const app = (module.exports = express())

  const error = (status, msg, additionalOutput = '') => {
    const err = new Error(msg)
    err.status = status
    err.additionalOutput = additionalOutput
    return err
  }

  app.use(bodyParser.json())

  // Super Simple Auth Middleware
  app.use((req, _, next) => {
    let authorization = req.headers['authorization']

    // key isn't present
    if (!authorization) return next(error(400, 'authorization required'))

    authToken = authorization.replace('Basic ', '')
    // key is invalid
    if (!~authKeys.indexOf(authToken)) return next(error(401, 'invalid api authorization'))

    // all good, store req.key for route access
    next()
  })

  app.get('/api/sensor', async (req, res, next) => {
    try {
      return res.status(200).json(await Sensor.find())
    } catch (err) {
      return error(500, err.message, { error: err })
    }
  })

  app.post('/api/sensor', async (req, res, next) => {
    const body = req.body
    try {
      const sensor = new Sensor({ ...body })
      await sensor.save()
      logger.info({ message: `Sensor ${body.name} created.` })
      res.send(sensor)
    } catch (err) {
      return error(500, err.message, { error: err })
    }
  })
  app.get('/api/sensor/:id', async (req, res, next) => {
    const _id = req.params.id
    try {
      const currentReadings = await Sensor.findOne({ _id })
      if (!currentReadings) return next(error(404, 'Sensor not found'))

      if (!currentReadings.populated('readings')) {
        logger.info({ message: 'currentReadings not populated' })
        await currentReadings.populate('readings').execPopulate()
      }
      if (currentReadings) res.send(currentReadings)
      else next()
    } catch (err) {
      next(error(500, err.message, { error: err }))
    }
  })

  app.post('/api/sensor/:id', async (req, res, next) => {
    const id = req.params.id
    const body = req.body
    try {
      const sensor = await Sensor.findById(id)
      if (!sensor) return error(404, 'Sensor not found')
      const reading = new Reading({ ...body, sensor: sensor._id })
      await reading.save()
      res.send(reading)
    } catch (err) {
      next(error(500, err.message, { error: err }))
    }
  })

  // Error Handler
  app.use((error, req, res, next) => {
    const { message, status, additionalOutput } = error
    errorLogger({
      message,
      status,
      additionalOutput,
    })
    res.status(status).send(message)
    next()
  })

  // default handler: 404
  app.use((req, res) => {
    return error(404, 'Not Found')
  })

  app.listen(3000, () => {
    logger.info({ message: 'Express started on port 3000' })
  })
}
const run = async () => {
  mongoose.set('debug', true)
  connection = await mongoose.connect('mongodb://192.168.1.213', {
    dbName: 'iot',
    auth: {
      authSource: 'iot',
    },
    user: 'iotUser', // Test user, replace with valid user from environment variable
    pass: 'GetMyReadings', // Test pass, replace with valid user from environment variable
    useNewUrlParser: true,
    useUnifiedTopology: true,
    autoCreate: true,
    useFindAndModify: false,
    useCreateIndex: true,
  })
  logger.info({ message: 'MongoDB database connection established successfully' })
  startApp()
}

run()
