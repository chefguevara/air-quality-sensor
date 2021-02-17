/**
 * Module dependencies.
 */
const authKeys = [ 'dGVzdDp0ZXN0' ] 

const mongoose = require('mongoose')
const express = require('express')
const bodyParser = require('body-parser')
const { Sensor, Reading } = require('./models')
const startApp = async () => {
  const app = module.exports = express()

const error = (status, msg) => {
  const err = new Error(msg)
  err.status = status
  return err
}

app.use(bodyParser.json())
// Auth Middleware
app.use((req, res, next) => {
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
  try{
    return res.status(200).json(await Sensor.find())
  } catch(err) {
    console.error(err)
    return res.status(500)
  }
})

app.post('/api/sensor', async (req, res, next) => {
  const body = req.body
  console.log('here')
  try{
    const sensor = new Sensor({...body})
    await sensor.save()
    console.log(`Sensor ${body.name} created.`)
    res.send( sensor)
  } catch(err) {
    console.error(err)
    return res.status(500)
  }
})
app.get('/api/sensor/:id', async (req, res, next) => {
  console.log('populate not populates')
  const _id = req.params.id
  const currentReadings = await Sensor.findOne({_id})
  
  if(!currentReadings.populated('readings')) {
    console.log('currentReadings not populated')
    await currentReadings.populate('readings').execPopulate()
  }

  console.log('this should be true', currentReadings.populated('readings'))
  if (currentReadings) res.send(currentReadings)
  else next()
})

app.post('/api/sensor/:id', async (req, res, next) => {
  const id = req.params.id
  const body = req.body
  try {
    const sensor = await Sensor.findById(id)
    if(!sensor) res.status(404).send({error: "Sensor not found"})
    const reading = new Reading({...body, sensor: sensor._id})
    await reading.save()
    res.send(reading)
  } catch(err){
    res.status(500).send(err)
  }
  
})

// Error Handler
app.use((err, req, res, next) => {
  res.status(err.status || 500)
  res.send({ error: err.message })
})

// default handler: 404 
app.use((req, res) => {
  res.status(404)
  res.send({ error: "Not Found" })
})


  app.listen(3000, ()=> {
    console.log('Express started on port 3000')
  })
}
const run = async () => {
mongoose.set('debug', true);
connection = await mongoose.connect('mongodb://192.168.1.213', {
  dbName: 'iot',
  auth: {
    authSource: "iot"
  },
  user: 'iotUser',
  pass: 'GetMyReadings',
  useNewUrlParser: true,
  useUnifiedTopology: true,
  autoCreate: true,
  useFindAndModify: false,
  useCreateIndex: true
})
  console.log("MongoDB database connection established successfully");
  startApp()
}

run()

