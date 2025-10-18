Function: int rl_on_new_line (void)
Tell the update functions that we have moved onto a new (empty) line, usually after outputting a newline.

rl_clear_history (void)
Clear the history list by deleting all of the entries, in the same manner as the History library’s clear_history() function. This differs from clear_history because it frees private data Readline saves in the history list.

Function: void rl_replace_line (const char *text, int clear_undo)
Replace the contents of rl_line_buffer with text. This preserves the point and mark, if possible. If clear_undo is non-zero, this clears the undo list associated with the current line.

Function: void rl_redisplay (void)
Change what’s displayed on the screen to reflect the current contents of rl_line_buffer.

accept-line (Newline or Return)
Accept the line regardless of where the cursor is. If this line is non-empty, you can add it to the history list using add_history(). If this line is a modified history line, then restore the history line to its original state.
