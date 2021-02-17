const { any } = require('bluebird')
const mongoose = require('mongoose')

const  Schema  =  mongoose.Schema 

const ReadingSchema = new Schema({
  sensor: { type: Schema.Types.ObjectId, ref: 'Sensor' },
  date: { type: Date, default: Date.now },
  data: {
    pressure: Schema.Types.Number,
    iaq: Schema.Types.Number,
    iaqAccuracy: Schema.Types.Number,
    temperature: Schema.Types.Number,
    humidity: Schema.Types.Number,
    co2Equivalent: Schema.Types.Number,
  }
})

module.exports = { ReadingSchema }