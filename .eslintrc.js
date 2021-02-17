module.exports = {
  extends: [
    'standard',
    'prettier',
    'prettier/standard',
    'plugin:prettier/recommended',
    'plugin:security/recommended',
  ],
  parserOptions: {
    ecmaVersion: 2019,
    sourceType: 'module',
    ecmaFeatures: {},
  },
  plugins: ['prettier', 'standard', 'security'],
  env: {
    'jest/globals': true,
  },
  globals: {
    fetch: false,
  },
}
