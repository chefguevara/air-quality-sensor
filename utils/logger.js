const chalk = require('chalk')
const { format, createLogger, transports } = require('winston')
const DailyRotateFile = require('winston-daily-rotate-file')
const safeStringify = require('fast-safe-stringify')
const { inspect } = require('util')
const { combine, timestamp, printf } = format

const formatLevel = (level) => {
  level = level.toUpperCase()
  switch (level) {
    case 'ERROR':
      return chalk.redBright(level)
    case 'WARN':
      return chalk.yellowBright(level)
    case 'INFO':
      return chalk.greenBright(level)
    case 'DEBUG':
      return chalk.magentaBright(level)
    case 'HTTP':
      return chalk.whiteBright(level)
  }
}
const fileTransport = new DailyRotateFile({
  filename: './logs/application-%DATE%.log',
  datePattern: 'YYYY-MM-DD-HH',
  zippedArchive: true,
  maxSize: '20m',
  maxFiles: '14d',
})

const consoleFormat = printf(
  ({ level, message, additionalOutput = '' }) =>
    `[${formatLevel(level)}] ${chalk.whiteBright(message)} ${
      !additionalOutput ? '' : JSON.stringify(safeStringify(additionalOutput))
    }`
)
const fileFormat = printf(
  ({ message, timestamp, additionalOutput = '' }) =>
    `${timestamp} - ${message} - ${
      !additionalOutput ? '' : inspect(JSON.stringify(safeStringify(additionalOutput)))
    }`
)

const logger = createLogger({
  level: 'info',
  format: consoleFormat,
  defaultMeta: { service: 'aiq-api' },
  transports: [new transports.Console()],
})

const errorLogger = createLogger({
  level: 'error',
  format: combine(timestamp(), fileFormat),
  defaultMeta: { service: 'aiq-api' },
  transports: [fileTransport],
})
module.exports = {
  logger,
  errorLogger: (params) => {
    logger.error(params)
    errorLogger.error(params) //TODO: include meta with request params
  },
}
