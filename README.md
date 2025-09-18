# minishell

| Feature            | Jasmine             | Káťa               |
|:-------------------|:--------------------|:-------------------|
| Signals            | ✔                   | (interface aid)    |
| Main Loop          | ✔                   |                    |
| Parsing            | ✔                   |(struct definitions)|
| Built-ins          | ✔                   |                    |
| Env Mgmt           | ✔                   | uses env           |
| Error Msg          | ✔                   | uses error         |
| Pipes              |                     | ✔                  |
| Redirs             |                     | ✔                  |
| Execution          |                     | ✔                  |
| Heredoc            | (collab)            | ✔                  |
| Cleanup            | (input, vars)       | (fds, children)    |

Test:
mdkir test
cd test
rm -rf ../test
pwd
-> has to stay in the current directory test

cmd1 + arg1 + arg2 etc :

you have to handle: ls > file.txt -la
to do the same as ls -la and putting it into a file