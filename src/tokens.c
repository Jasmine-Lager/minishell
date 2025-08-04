// The lexer breaks input strings into meaningful tokens while handling quote
// parsing and special characters. This stage transforms raw input like
// echo "hello world" | wc-l > output.txt into discrete tokens.

// COMMAND, ARGUMENT, PIPE, REDIRECT_IN, REDIRECT_OUT, etc.
// Handle single and double quotes appropriately
// Manage whitespace and special character separation
// Store tokens in a linked list or array structure
