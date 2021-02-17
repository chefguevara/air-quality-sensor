const mongoose = require('mongoose')
const {SensorSchema} = require('../schemas')

module.exports = {
  Sensor: mongoose.model('Sensor', SensorSchema)
}