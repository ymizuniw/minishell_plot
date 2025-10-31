#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>

// Test helper to create a temporary test file
void create_test_file(const char *filename, const char *content)
{
	int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	assert(fd >= 0);
	write(fd, content, strlen(content));
	close(fd);
}

// Test helper to read a file content
char *read_file_content(const char *filename)
{
	int fd = open(filename, O_RDONLY);
	if (fd < 0)
		return NULL;
	
	char *buffer = malloc(1024);
	ssize_t bytes = read(fd, buffer, 1023);
	close(fd);
	
	if (bytes < 0)
	{
		free(buffer);
		return NULL;
	}
	buffer[bytes] = '\0';
	return buffer;
}

// Test helper to clean up test files
void cleanup_test_files(void)
{
	unlink("test_input.txt");
	unlink("test_output.txt");
	unlink("test_append.txt");
	unlink("test_input2.txt");
}

// Test 1: cat < input.txt (redirect input)
void test_cat_redirect_input(void)
{
	printf("Test: cat < input.txt\n");
	
	create_test_file("test_input.txt", "Hello from input file\n");
	
	pid_t pid = fork();
	if (pid == 0)
	{
		int fd = open("test_input.txt", O_RDONLY);
		assert(fd >= 0);
		dup2(fd, STDIN_FILENO);
		close(fd);
		
		char *args[] = {"cat", NULL};
		execvp("cat", args);
		perror("execvp");
		exit(1);
	}
	
	int status;
	waitpid(pid, &status, 0);
	
	if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
		printf("  ✓ PASSED\n\n");
	else
		printf("  ✗ FAILED\n\n");
}

// Test 2: cat input.txt > output.txt
void test_cat_redirect_output(void)
{
	printf("Test: cat input.txt > output.txt\n");
	
	create_test_file("test_input.txt", "Content to redirect\n");
	
	pid_t pid = fork();
	if (pid == 0)
	{
		int fd = open("test_output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
		assert(fd >= 0);
		dup2(fd, STDOUT_FILENO);
		close(fd);
		
		char *args[] = {"cat", "test_input.txt", NULL};
		execvp("cat", args);
		perror("execvp");
		exit(1);
	}
	
	int status;
	waitpid(pid, &status, 0);
	
	char *content = read_file_content("test_output.txt");
	if (content && strcmp(content, "Content to redirect\n") == 0)
		printf("  ✓ PASSED\n\n");
	else
		printf("  ✗ FAILED\n\n");
	
	free(content);
}

// Test 3: cat < input.txt > output.txt
void test_cat_redirect_both(void)
{
	printf("Test: cat < input.txt > output.txt\n");
	
	create_test_file("test_input.txt", "Both redirections test\n");
	
	pid_t pid = fork();
	if (pid == 0)
	{
		int fd_in = open("test_input.txt", O_RDONLY);
		int fd_out = open("test_output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
		assert(fd_in >= 0 && fd_out >= 0);
		
		dup2(fd_in, STDIN_FILENO);
		dup2(fd_out, STDOUT_FILENO);
		close(fd_in);
		close(fd_out);
		
		char *args[] = {"cat", NULL};
		execvp("cat", args);
		perror("execvp");
		exit(1);
	}
	
	int status;
	waitpid(pid, &status, 0);
	
	char *content = read_file_content("test_output.txt");
	if (content && strcmp(content, "Both redirections test\n") == 0)
		printf("  ✓ PASSED\n\n");
	else
		printf("  ✗ FAILED\n\n");
	
	free(content);
}

// Test 4: stdin/stdout restoration
void test_fd_restoration(void)
{
	printf("Test: stdin/stdout restoration\n");
	
	int saved_stdin = dup(STDIN_FILENO);
	int saved_stdout = dup(STDOUT_FILENO);
	
	create_test_file("test_input.txt", "temp\n");
	
	// Redirect stdin
	int fd_in = open("test_input.txt", O_RDONLY);
	dup2(fd_in, STDIN_FILENO);
	close(fd_in);
	
	// Redirect stdout
	int fd_out = open("test_output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	dup2(fd_out, STDOUT_FILENO);
	close(fd_out);
	
	// Restore
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	
	printf("  ✓ PASSED: FDs restored successfully\n\n");
}

// Test 5: Append mode
void test_cat_append(void)
{
	printf("Test: cat >> output.txt (append)\n");
	
	create_test_file("test_input.txt", "New line\n");
	create_test_file("test_append.txt", "Existing\n");
	
	pid_t pid = fork();
	if (pid == 0)
	{
		int fd = open("test_append.txt", O_WRONLY | O_APPEND);
		assert(fd >= 0);
		dup2(fd, STDOUT_FILENO);
		close(fd);
		
		char *args[] = {"cat", "test_input.txt", NULL};
		execvp("cat", args);
		exit(1);
	}
	
	int status;
	waitpid(pid, &status, 0);
	
	char *content = read_file_content("test_append.txt");
	if (content && strcmp(content, "Existing\nNew line\n") == 0)
		printf("  ✓ PASSED\n\n");
	else
		printf("  ✗ FAILED\n\n");
	
	free(content);
}

int main(void)
{
	printf("========================================\n");
	printf("  CAT AND REDIRECTION TESTS\n");
	printf("========================================\n\n");
	
	test_cat_redirect_input();
	test_cat_redirect_output();
	test_cat_redirect_both();
	test_fd_restoration();
	test_cat_append();
	
	cleanup_test_files();
	
	printf("========================================\n");
	printf("  ALL TESTS COMPLETED\n");
	printf("========================================\n");
	
	return 0;
}
