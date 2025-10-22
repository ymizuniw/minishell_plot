#include "../../includes/minishell.h"

//search not-built-in command path.

int search_path(char **argv, char **env)
{
    char *command_name = argv[0];
    char *path = getenv(command_name);
    if (path==NULL)
        return (NULL);
    else
    {
        argv[]
    }
}
You’ve got the skeleton of an `exec_plot()` and traversal routine that roughly matches how Bash or a POSIX‐style shell walks an AST, but several lines are unsafe or semantically wrong.  Here’s a breakdown of the main problems and what to fix so the executor can actually run commands correctly and obey `&&`, `||`, and pipelines.


And for non-pipeline nodes, don’t touch FDs.

---

### 5. `ast_traversal()` logic

Right now it forks both left and right unconditionally, which doubles processes and doesn’t honor sequencing.

You need:

1. Execute left child first.
2. Use its exit status to decide if right child executes (for `&&`/`||`).
3. For pipelines, create pipe() before forking left/right.

Skeleton:

```c
int	ast_traversal(t_ast *node, char **env)
{
	int status = 0;

	if (!node)
		return 0;

	if (node->type == NODE_AND)
	{
		status = ast_traversal(node->left, env);
		if (status == 0)
			status = ast_traversal(node->right, env);
	}
	else if (node->type == NODE_OR)
	{
		status = ast_traversal(node->left, env);
		if (status != 0)
			status = ast_traversal(node->right, env);
	}
	else if (node->type == NODE_PIPE)
		status = exec_pipe(node, env);//fork() left node process, and then fork() right node process.
	else if (node->type == NODE_SUBSHELL)
		status = exec_subshell(node, env);
	else if (node->type == NODE_CMD)
		status = exec_command(node, env);
	return status;
}
```

---

### 6. Missing helper separation

Break executor into:

| Function          | Role                                |
| ----------------- | ----------------------------------- |
| `exec_command()`  | do redirections + `execve`          |
| `exec_pipe()`     | create `pipe()`, fork left/right    |
| `exec_subshell()` | `fork()` + run subtree              |
| `ast_traversal()` | recursion controller (&&, ||, etc.) |

That structure prevents recursion from forking the same node twice.

---

### 7. Example minimal execution flow

```c
int	exec_command(t_ast *node, char **env)
{
	char *path = path_validation(node->cmd->argv[0], env);

	if (!path)
		return 127; /* command not found */
	do_redirection(node);
	if (execve(path, node->cmd->argv, env) == -1)
		perror(path);
	_exit(errno);
}
```

---

### 8. Memory / resource safety

* Always close file descriptors after dup2().
* Never call `execve()` in parent process — only child.
* When using `fork()`, call `_exit()` in children, not `return`.

---

### ✅ Summary of priority fixes

| Area                    | Problem                                     | Fix                                |
| ----------------------- | ------------------------------------------- | ---------------------------------- |
| `exec_plot()`           | duplicate variables, recursion on same node | remove duplicates, recurse right   |
| `do_redirection()`      | wrong flags & dup2 usage                    | correct open modes & FDs           |
| `do_pipe_redirection()` | wrong `stdin`/`stdout` usage                | use `STDIN_FILENO`/`STDOUT_FILENO` |
| `ast_traversal()`       | excessive forks, wrong sequencing           | single fork per execution step     |
| `execve()` call         | argv[0] passed                              | pass full `argv` array             |

---

If you want, I can rewrite the executor into a fully correct POSIX-style form that matches your AST structure (`NODE_AND`, `NODE_OR`, `NODE_PIPE`, `NODE_CMD`, `NODE_SUBSHELL`) and uses the same token direction rules. Would you like that version next?
