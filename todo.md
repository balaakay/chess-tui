# TODO

## Gameplay Implementation
- [ ] parse move string
- [ ] update game state based on move ***HARD***
    - [ ] check if move is possible
    - [ ] 
- [ ] check for check/mate (should be specified in the move but check anyways)
- [ ] provide feedback for the user
    - [ ] Show most recent move (valuable for multiplayer)
    - [ ] show current material breakdown + advantage
    - [ ] show other important values on the screen (basically print all data from FEN)
        - [ ] halfmove clock
        - [ ] who can castle and where, if any
        - [ ] en passant square, if any 
        - [ ] full move clock


## Multiplayer Implementation
- [ ] Start menu
- [ ] gameplay loop
- [ ] multiplayer option to copy/paste FENs across different terminals


## Testing
- [ ] find_square_name's 64 cases + impossibles that should throw errors




## ADVANCED
- [ ] incorporation of very basic chess engine
- [ ] tui version that adapts to size of terminal (spacing of different UI
  elements changes based on character width allowed
- [ ] is it possible to connect 2 terminal sessions together and allow a game to
  play amongst them? or would this require a 'server' and 'client' architecture?
