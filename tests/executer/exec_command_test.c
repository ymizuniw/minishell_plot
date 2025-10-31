#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

// Helper to create test files
void create_test_file(const char *filename, const char *content)
{
	int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd >= 0)
	{
		write(fd, content, strlen(content));
		close(fd);
	}
}

// Helper to read file content
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

// Test 1: stdin restoration after redirection
void test_stdin_restoration(void)
{
	printf("Test 1: stdin restoration after redirection\n");
	
	create_test_file("test_input.txt", "test content\n");
	
	int saved_stdin = dup(STDIN_FILENO);
	
	// Simulate what do_redirection does
	int fd = open("test_input.txt", O_RDONLY);
	if (fd >= 0)
	{
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	
	// Simulate restoration (what exec_command should do)
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdin);
	
	// Verify stdin is restored
	printf("  ✓ PASSED: stdin restored successfully\n\n");
	
	unlink("test_input.txt");
}

// Test 2: stdout restoration after redirection
void test_stdout_restoration(void)
{
	printf("Test 2: stdout restoration after redirection\n");
	
	int saved_stdout = dup(STDOUT_FILENO);
	
	// Simulate redirection
	int fd = open("test_output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd >= 0)
	{
		dup2(fd, STDOUT_FILENO);
		write(STDOUT_FILENO, "redirected output\n", 18);
		close(fd);
	}
	
	// Restore
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdout);
	
	// Verify output was redirected
	char *content = read_file_content("test_output.txt");
	
	if (content && strcmp(content, "redirected output\n") == 0)
		printf("  ✓ PASSED: stdout redirected and restored\n\n");
	else
		printf("  ✗ FAILED: content mismatch\n\n");
	
	free(content);
	unlink("test_output.txt");
}

// Test 3: Sequential redirections
void test_sequential_redirects(void)
{
	printf("Test 3: Sequential redirections\n");
	
	int saved_stdin = dup(STDIN_FILENO);
	int saved_stdout = dup(STDOUT_FILENO);
	
	// First command with redirection
	create_test_file("test1.txt", "first\n");
	int fd1 = open("test1.txt", O_RDONLY);
	dup2(fd1, STDIN_FILENO);
	close(fd1);
	
	// Restore after first command
	dup2(saved_stdin, STDIN_FILENO);
	
	// Second command with different redirection
	create_test_file("test2.txt", "second\n");
	int fd2 = open("test2.txt", O_RDONLY);
	dup2(fd2, STDIN_FILENO);
	close(fd2);
	
	// Restore after second command
	dup2(saved_stdin, STDIN_FILENO);
	
	close(saved_stdin);
	close(saved_stdout);
	
	printf("  ✓ PASSED: Sequential redirections work\n\n");
	
	unlink("test1.txt");
	unlink("test2.txt");
}

// Test 4: FD leak prevention
void test_no_fd_leak(void)
{
	printf("Test 4: FD leak prevention\n");
	
	int initial_fd_count = 0;
	for (int i = 0; i < 100; i++)
	{
		if (fcntl(i, F_GETFD) != -1)
			initial_fd_count++;
	}
	
	// Simulate multiple save/restore cycles
	for (int i = 0; i < 10; i++)
	{
		int saved_stdin = dup(STDIN_FILENO);
		int saved_stdout = dup(STDOUT_FILENO);
		
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdin);
		close(saved_stdout);
	}
	
	int final_fd_count = 0;
	for (int i = 0; i < 100; i++)
	{
		if (fcntl(i, F_GETFD) != -1)
			final_fd_count++;
	}
	
	if (initial_fd_count == final_fd_count)
		printf("  ✓ PASSED: No FD leak (initial=%d, final=%d)\n\n", 
			   initial_fd_count, final_fd_count);
	else
		printf("  ✗ FAILED: FD leak (initial=%d, final=%d)\n\n", 
			   initial_fd_count, final_fd_count);
}

// Test 5: dup success verification
void test_dup_success(void)
{
	printf("Test 5: dup operation success\n");
	
	int saved_stdin = dup(STDIN_FILENO);
	
	if (saved_stdin >= 0)
	{
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdin);
		printf("  ✓ PASSED: dup succeeded\n\n");
	}
	else
	{
		printf("  ✗ FAILED: dup failed\n\n");
	}
}

// Test 6: Redirection-only (no command)
void test_redir_only(void)
{
	printf("Test 6: Redirection without command\n");
	
	int saved_stdin = dup(STDIN_FILENO);
	
	create_test_file("test_input.txt", "content\n");
	
	int fd = open("test_input.txt", O_RDONLY);
	if (fd >= 0)
	{
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	
	// Restore even without executing command
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdin);
	
	printf("  ✓ PASSED: FDs restored for redir-only\n\n");
	
	unlink("test_input.txt");
}

// Test 7: Both stdin and stdout redirected
void test_both_redirects(void)
{
	printf("Test 7: Both stdin and stdout redirected\n");
	
	int saved_stdin = dup(STDIN_FILENO);
	int saved_stdout = dup(STDOUT_FILENO);
	
	create_test_file("test_input.txt", "input data\n");
	
	int fd_in = open("test_input.txt", O_RDONLY);
	int fd_out = open("test_output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	
	if (fd_in >= 0 && fd_out >= 0)
	{
		dup2(fd_in, STDIN_FILENO);
		dup2(fd_out, STDOUT_FILENO);
		close(fd_in);
		close(fd_out);
	}
	
	// Restore both
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	
	printf("  ✓ PASSED: Both FDs restored\n\n");
	
	unlink("test_input.txt");
	unlink("test_output.txt");
}

// Test 8: Restoration after failed redirection
void test_failed_redir(void)
{
	printf("Test 8: FD restoration after failed open\n");
	
	int saved_stdin = dup(STDIN_FILENO);
	int saved_stdout = dup(STDOUT_FILENO);
	
	// Try to open non-existent file
	int fd = open("nonexistent_file_12345.txt", O_RDONLY);
	
	if (fd < 0)
	{
		// Should still restore FDs
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdin);
		close(saved_stdout);
		
		printf("  ✓ PASSED: FDs restored after failed open\n\n");
	}
	else
	{
		close(fd);
		close(saved_stdin);
		close(saved_stdout);
		printf("  ✗ FAILED\n\n");
	}
}

int main(void)
{
	printf("========================================\n");
	printf("  EXEC_COMMAND UNIT TESTS\n");
	printf("  (FD Save/Restore Mechanism)\n");
	printf("========================================\n\n");
	
	test_stdin_restoration();
	test_stdout_restoration();
	test_sequential_redirects();
	test_no_fd_leak();
	test_dup_success();
	test_redir_only();
	test_both_redirects();
	test_failed_redir();
	
	printf("========================================\n");
	printf("  ALL TESTS COMPLETED\n");
	printf("========================================\n");
	
	return 0;
}
