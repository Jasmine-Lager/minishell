// The executor creates processes, establishes pipes, handles redirections,
// and manages command execution.This stage orchestrates the actual work.

// Execution Components :

// Process creation with
// fork() / execve()
// Pipe establishment and management
// File descriptor redirection
// Built in command detection and handling
// 	Step-by-Step Implementation Approach