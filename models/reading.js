const mongoose = require('mongoose')
const { ReadingSchema } = require('../schemas')

module.exports = {
  Reading: mongoose.model('Reading', ReadingSchema),
}
