# Pipex

Pipex is a project from 42's Common Core. This is my own implementation that has successfully passed the 42 project requirements. It executes a series of commands connected by pipes, handling input/output redirections (including heredoc), and ensures robust error management and resource cleanup. All memory allocations are centralized in a pipeline structure that is properly freed before the program terminates if initialized.

---

## Table of Contents

- [Features](#features)
- [Cloning](#cloning)
- [Compilation](#compilation)
- [Usage](#usage)
- [Data Structures](#data-structures)
- [Error Management and Resource Handling](#error-management-and-resource-handling)
- [Project Structure](#project-structure)
- [Mermaid Diagrams](#mermaid-diagrams)
- [Repository](#repository)
- [License](#license)

---

## Features

- **Command Execution:** Uses `fork()`, `execve()`, and `pipe()` to build a chain of processes.
- **Redirection Handling:** Implements input, output, append, and heredoc redirections.
- **Error Management:** Centralized error management via `error_exit_code()` ensures that errors are reported and resources are released.
- **Memory Management:** All memory allocations for the pipeline are stored in the pipeline structure and are freed upon termination.
- **Resource Safety:** Always closes file descriptors to avoid leaks and prevent hanging reads/writes.
- **Bonus Mode:** Supports a bonus mode using heredoc input.

---

## Cloning

```bash
git clone --recursive https://github.com/cesardelarosa/pipex.git
```

If you did not clone it with the `--recursive` flag, you can initialize and update submodules manually:

```bash
git submodule init
git submodule update
```

---

## Compilation

To build the standard version, run:
```bash
make
```

To build the bonus version (with heredoc support), run:
```bash
make bonus
```

This will create the `pipex` or `pipex_bonus` executables in the project root.

---

## Usage

### Standard Mode

```bash
./pipex infile "cmd1" "cmd2" outfile
```

### Bonus Mode (with heredoc)

```bash
./pipex here_doc LIMITER "cmd1" "cmd2" ... outfile
```

**Note:** The program validates its arguments and will print a usage message and exit if the required number of arguments is not provided.

---

## Data Structures

This project defines three primary structures in [structs.h](include/structs.h) to organize pipeline data and manage commands:

1. **`t_pipeline`**  
   - Maintains a list of commands (`commands`), the number of commands (`cmd_count`), an array of pipes (`pipes`), and an array of pids (`pids`).  
   - Provides a central place for resource allocation and cleanup.
2. **`t_command`**  
   - Holds the argument list (`argv`), a list of redirections (`redirs`), and a pointer to the parent pipeline (`p`).  
   - Each command can have multiple redirections attached.
3. **`t_redir`**  
   - Describes a redirection with a type (`type`), a file name (`file`), and a reference to the command that owns it (`cmd`).  
   - Supported types include: `REDIR_INPUT`, `REDIR_OUTPUT`, `REDIR_APPEND`, `REDIR_HEREDOC`.

Below is a Mermaid class diagram illustrating the relationships among these structures:

```mermaid
classDiagram
	class t_list {
		+void *content
		+t_list *next
	}

	class t_pipeline {
		+t_list *commands  // List of t_command
		+unsigned int cmd_count
		+int **pipes
		+pid_t *pids
	}

	class t_command {
		+char **argv
		+t_list *redirs  // List of t_redir
		+t_pipeline *p
	}

	class t_redir {
		+t_redir_type type
		+char *file
		+t_command *cmd
	}

	%% Relationships
	t_pipeline --> t_list : commands
	t_list --> t_command : stores
	t_command --> t_list : redirs
	t_list --> t_redir : stores

```

These data structures ensure clear ownership of memory and file descriptors, allowing for robust cleanup in case of errors or normal termination.

---

## Error Management and Resource Handling

- **Error Management:**  
  The function `error_exit_code()` is used throughout the project to print a meaningful error message, free allocated resources (by calling `pipeline_destroy()`), and exit with a specific code.
  
- **File Descriptors:**  
  All file descriptors created (for pipes and redirections) are explicitly closed when no longer needed using helper functions like `close_all_pipes()` and `safe_close()`.
  
- **Memory Management:**  
  All memory allocations are stored in the `t_pipeline` structure. Before the program ends, whether due to successful execution or an error, the pipeline structure is completely freed. This centralization ensures that memory leaks are avoided even if an error occurs.

---

## Project Structure

```
/pipex
├── include/            # Header files for the project
├── libft/              # Libft library (compiled as libft.a)
├── obj/                # Object files (created by the Makefile)
├── src/                # Source files (command creation, execution, error handling, etc.)
├── Makefile            # Build instructions
└── README.md           # This file
```

---

## Mermaid Diagrams

Below are additional Mermaid diagrams that illustrate the key functions and their control flow.

### 1. `execute_command` Flow

This diagram details the process within the execute_command function. It handles the command's redirections and then proceeds to find and execute the appropriate executable. In case of failures (like redirection issues or missing executables), it triggers corresponding error routines.

```mermaid
flowchart TD
    A["execute_command(cmd, envp)"]
    A --> B["handle_redirs(cmd->redirs)"]
    B --> C{"Result < 0?"}
    C -- "Yes" --> D["error_exit_code('redirection failed')"]
    C -- "No" --> E["Call find_executable(cmd->argv[0], envp)"]
    E --> F{"Executable found?"}
    F -- "No" --> G["error_exit_code(127, 'Command not found')"]
    F -- "Yes" --> H["execve(path, cmd->argv, envp)"]
    H --> I["free(path)"]
    I --> J{"Check errno"}
    J -- "ENOENT" --> K["error_exit_code(127, 'Command not found')"]
    J -- "EACCES" --> L["error_exit_code(126, 'Permission denied')"]
    J -- "Other" --> M["error_exit_code(126, strerror(errno))"]

    %% Subgraph: find_executable flow
    subgraph FIND_EXEC["find_executable"]
        direction TB
        N1["Check if cmd is empty"]
        N1 --> N2{"Is empty?"}
        N2 -- "Yes" --> N3["Return NULL"]
        N2 -- "No" --> N4["Check if cmd contains '/'"]
        N4 -- "Yes" --> N5["Return ft_strdup(cmd)"]
        N4 -- "No" --> N6["Call get_env_value('PATH', envp)"]
        N6 --> N7{"PATH found?"}
        N7 -- "No" --> N8["Return ft_strjoin('./', cmd)"]
        N7 -- "Yes" --> N9["Split PATH into paths (ft_split)"]
        N9 --> N10["Call search_in_path(cmd, paths)"]
        N10 --> N11["Free paths (ft_free_split)"]
        N11 --> N12["Return exec_path"]
    end
    E --- FIND_EXEC
```

### 2. `pipeline_execute` Flow

This diagram illustrates the flow of the pipeline_execute function, which is responsible for managing the execution of multiple commands as a pipeline. It covers memory allocation for process IDs, pipe creation, forking for each command, setting up child process pipes, executing commands, and finally waiting for all child processes to finish to aggregate their exit statuses.

```mermaid
flowchart TD
    A["pipeline_execute(p, envp)"]
    A --> B["Allocate memory for p->pids"]
    B --> C{"malloc successful?"}
    C -- No --> D["error_exit_code('malloc failed')"]
    C -- Yes --> E["create_pipes(p)"]
    E --> F{"Pipes created successfully?"}
    F -- No --> G["return 0"]
    F -- Yes --> H["Initialize iteration over p->commands"]
    H --> I["For each command (index: 0 to cmd_count-1)"]
    I --> J["fork_command(cmd, index, envp)"]
    J --> K{"fork() result"}
    K -- "pid < 0" --> L["error_exit_code('fork failed')"]
    K -- "pid > 0" --> M["Parent: Save pid in p->pids[index]"]
    K -- "pid == 0" --> N["Child Process"]
    
    %% Child Branch
    N --> O["setup_child_pipes(index, p)"]
    O --> P["execute_command(cmd, envp)"]
    P --> Q{"Execution successful?"}
    Q -- "No" --> R["error_exit_code('execution failed')"]
    Q -- "Yes" --> S["Child completes execution"]
    
    %% Back to Parent Iteration
    M --> T["Increment index and process next command"]
    T --> I
    
    %% After iteration
    I -- "All commands processed" --> U["Parent: close_all_pipes(p)"]
    U --> V["Parent: wait_for_children(p)"]
    
    %% wait_for_children Branch
    V --> W["For each pid in p->pids:"]
    W --> X["waitpid(pid, &status, 0)"]
    X --> Y{"Termination Type"}
    Y -- "Normal exit" --> Z["Obtain exit status (WEXITSTATUS)"]
    Y -- "Terminated by signal" --> AA["Calculate exit status: 128 + signal"]
    Z & AA --> AB["Aggregate final exit_status"]
    AB --> AC["Return exit_status from pipeline_execute"]
```

### 3. General Program Flow

This diagram outlines the overall control flow of the program starting from the main_bonus function. It demonstrates the initial argument validation, mode determination (whether to run in here_doc mode or normal mode), pipeline initialization, execution of the pipeline, and the subsequent cleanup of resources once execution is complete.

```mermaid
flowchart TD
    A["main(argc, argv, envp)"]
    A --> B["validate_args(argc, argv)"]
    B --> C{"Arguments valid?"}
    C -- "No" --> D["Display usage & exit"]
    C -- "Yes" --> E["Determine mode: 'here_doc' or normal"]
    E -- "here_doc" --> F["init_heredoc(pipeline, argc, argv)"]
    E -- "normal" --> G["init_normal(pipeline, argc, argv)"]
    F --> H["Pipeline initialized"]
    G --> H["Pipeline initialized"]
    H --> I{"Initialization successful?"}
    I -- "No" --> J["error_exit_code('allocation error')"]
    I -- "Yes" --> K["pipeline_execute(pipeline, envp)"]
    
    %% Detail in pipeline_execute
    K --> L["Create pipes & fork commands"]
    L --> M["For each command:"]
    M --> N["Child process:\nsetup_child_pipes() -> execute_command()"]
    N --> O{"Execution error?"}
    O -- "Yes" --> P["error_exit_code(...)\nin child"]
    O -- "No" --> Q["Child completes execution"]
    M --> R["Parent process:\nSave child PID"]
    R --> S["After loop: close_all_pipes()"]
    S --> T["wait_for_children()\n(Aggregate exit_status)"]
    
    T --> U["pipeline_destroy(pipeline)"]
    U --> V["Return final exit_status"]
```

---

## Repository

The complete project is hosted on GitHub:

[github.com/cesardelarosa/pipex](https://github.com/cesardelarosa/pipex)

---

## License

This project is licensed under the GPLv3 License. See the [LICENSE](LICENSE) file for details.
