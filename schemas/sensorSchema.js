const mongoose = require('mongoose')

const  Schema = mongoose.Schema

const SensorSchema = new Schema({
  // _id: Schema.Types.ObjectId,
  name: String,
  description: String,
  readings: [{ type: Schema.Types.ObjectId, ref: 'Reading' }],
})

module.exports = { SensorSchema } 