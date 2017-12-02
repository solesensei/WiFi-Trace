/*       foreground background
black        30         40
red          31         41
green        32         42
yellow       33         43
blue         34         44
magenta      35         45
cyan         36         46
white        37         47

reset             0  (everything back to normal)
bold/bright       1  (often a brighter shade of the same colour)
underline         4
inverse           7  (swap foreground and background colours)
bold/bright off  21
underline off    24
inverse off      27
*/

#define reset "\033[0m" // reset color 
#define rcol "\033[31m" // red
#define gcol "\033[32m" // green
#define ycol "\033[33m" // yellow
#define bcol "\033[34m" // blue
#define mcol "\033[35m" // magenta
#define ccol "\033[36m" // cyan
#define wcol "\033[37m" // white

#define rbcol "\033[1;31m" // red bold
#define gbcol "\033[1;32m" // green bold
#define ybcol "\033[1;33m" // yellow bold
#define bbcol "\033[1;34m" // blue bold
#define mbcol "\033[1;35m" // magenta bold
#define cbcol "\033[1;36m" // cyan bold
#define wbcol "\033[1;37m" // white bold
