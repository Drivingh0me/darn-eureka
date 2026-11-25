#ifndef ERROR_H
#define ERROR_H

#define ERR_MSG_LEN_MAX 255

/* enum and union for global error types of int.
 * Each function cleans itself up with goto to unwind from fatal error.
 */
typedef enum {
    MSG = -6,
    MEM_ALLOC = -5,
    IO = -4,
    INVALID_ARG = -3,
    INVALID_VAL = -2,
    EMPTY_ARG = -1,
} Error;

// /* Last error state gets printed */
// typedef struct {
//     int err;
//     char errMsg[ERR_MSG_LEN_MAX];
// } ErrorState;

/* Does not cleanup, just reports error */
void app_err(Error errno, char* message);
void throw_err(Error err);
// void add_err_msg(ErrorState errSt, char *str);
// void throw_err(int err);

#endif /* ERROR_H */
