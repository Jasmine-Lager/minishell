// The expander handles variable substitution, tilde expansion,
// and quote removal before execution. This stage processes $HOME, $?,
// and environment variables.

// Expansion Tasks (Variable Processing):
// Environment variable substitution ($VAR)
// Exit status expansion ($?)
// Home directory expansion (~)
// Quote removal after expansion
