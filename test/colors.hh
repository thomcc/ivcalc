#ifndef __COLORS_HH__
#define __COLORS_HH__

#define COL_RESET "\e[0m"
#define BOLD_ON "\e[1m"
#define UNDERLINE "\e[4m"
#define BLINK "\e[5m"
#define NEGATIVE "\e[7m"
#define BOLD_OFF "\e[22m"
#define UNDERLINE_OFF "\e[24m"
#define BLINK_OFF "\e[25m"
#define NEGATIVE_OFF "\e[27m"
#define FG_BLACK "\e[30m"
#define FG_RED "\e[31m"
#define FG_GREEN "\e[32m"
#define FG_YELLOW "\e[33m"
#define FG_BLUE "\e[34m"
#define FG_MAGENTA "\e[35m"
#define FG_CYAN "\e[36m"
#define FG_WHITE "\e[37m"
#define FG_DEFAULT "\e[39m"
#define BG_BLACK "\e[40m"
#define BG_RED "\e[41m"
#define BG_GREEN "\e[42m"
#define BG_YELLOW "\e[43m"
#define BG_BLUE "\e[44m"
#define BG_MAGENTA "\e[45m"
#define BG_CYAN "\e[46m"
#define BG_WHITE "\e[47m"
#define BG_DEFAULT "\e[49m"


#define BLACK(S) FG_BLACK S FG_DEFAULT
#define RED(S) FG_RED S FG_DEFAULT
#define GREEN(S) FG_GREEN S FG_DEFAULT
#define YELLOW(S) FG_YELLOW S FG_DEFAULT
#define BLUE(S) FG_BLUE S FG_DEFAULT
#define MAGENTA(S) FG_MAGENTA S FG_DEFAULT
#define CYAN(S) FG_CYAN S FG_DEFAULT
#define WHITE(S) FG_WHITE S FG_DEFAULT
#define DEFAULT(S) FG_DEFAULT S FG_DEFAULT

#define BOLD(S) BOLD_ON S BOLD_OFF

#define K_BOLD(S) BLACK(BOLD(S))
#define R_BOLD(S) RED(BOLD(S))
#define G_BOLD(S) GREEN(BOLD(S))
#define Y_BOLD(S) YELLOW(BOLD(S))
#define B_BOLD(S) BLUE(BOLD(S))
#define M_BOLD(S) MAGENTA(BOLD(S))
#define C_BOLD(S) CYAN(BOLD(S))
#define W_BOLD(S) WHITE(BOLD(S))
#define D_BOLD(S) DEFAULT(BOLD(S))

#endif