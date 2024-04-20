// GUI to Engine commands
#define UCI "uci"              // Universal Chess Interface
#define DEBUG "debug"          // Debug mode [on | off]
#define IS_READY "isready"     // Check if the engine is ready
#define SET_OPTION "setoption" // setoption name <id> [value <x>]
#define REGISTER "register"
#define UCI_NEW_GAME "ucinewgame" // Following commands are for a new game
#define POSITION "position" // position [fen <fenstring> | startpos] moves <move1> .... <movei>
#define GO                                                                                         \
  "go" // go [searchmoves <move1> .... <movei>] [ponder] [wtime <x>] [btime <x]] [winc <x>]
       // [binc <x>] [movestogo <x>] [depth <x>] [nodes <x>] [mate <x>] [movetime <x>] [infinite]
#define STOP "stop" // Stop calculating as soon as possible
#define PONDER_HIT "ponderhit"
#define QUIT "quit" // Quit the program as soon as possible

// Engine to GUI commands
#define ID "id"
#define UCI_OK "uciok"
#define READY_OK "readyok"
#define BEST_MOVE "bestmove"
#define COPY_PROTECTION "copyprotection"
#define REGISTRATION "registration"
#define INFO "info"
#define OPTION "option"
