% Dabrowski Damian, 439954
:- ensure_loaded(library(lists)).
:- op(700, xfx, '<>').
L <> R :- L \= R.

% Entry point
verify :-
   current_prolog_flag(argv, Argv),
   ( validate_arguments(Argv, N, File) ->
     verify(N, File)
   ; format('Error: bledne argumenty programu~n', [])
   ).

validate_arguments(Argv, N, File) :-
    length(Argv, 3),
    nth0(1, Argv, ArgN),
    nth0(2, Argv, File),
    atom_number(ArgN, N).

% Main verification logic
verify(N, File) :-
   integer(N),
   N > 0,
   set_prolog_flag(fileerrors, off),
   (  convert_to_atom(File, ActualFile) ->
      (  see(ActualFile) ->
         read_program(Prog),
         seen,
         initState(Prog, N, State),
         generateSteps(Prog, State, IsSafe),
         printResult(IsSafe)
      ; wrongFile(File)
      )
   ;  wrongFile(File)
   ).

% Handle incorrect N values
verify(N, _) :-
   format('Error: parametr ~w powinien byc liczba > 0~n', [N]).

% Helper predicate to convert strings to atoms if necessary
convert_to_atom(Input, Atom) :-
   (   atom(Input)
   ->  Atom = Input
   ;   is_list(Input)      % sicstus on students has no string/1
   ->  atom_codes(Atom, Input)
   ).

% Read program from file
read_program(program(Variables, Arrays, Instr)) :-
   read(variables(Variables)),
   read(arrays(Arrays)),
   read(program(Instr)).

% Handle file errors
wrongFile(File) :-
   format('Error: brak pliku o nazwie - ~w~n', [File]).

% printResult(+IsSafe)
printResult(safe) :-
   format('Program jest poprawny (bezpieczny).~n', []).

printResult(unsafe(Steps, Procs)) :-
   format('Program jest niepoprawny.~nNiepoprawny przeplot:~n', []),
   printSteps(Steps),
   format('Procesy w sekcji:', []),
   printCollision(Procs).

% Print a list of steps until collision.
printSteps([]).
printSteps([PrId-INr|T]) :-
   format('    Proces ~d: ~d~n', [PrId, INr]),
   printSteps(T).

% Print a list of processes in critical section (in collision).
printCollision([P]) :-
   format(' ~d.~n', [P]).
printCollision([P|Ps]) :-
   format(' ~d,', [P]),
   printCollision(Ps).

% initState(+Prog, +N, -InitState)
% Program representation: program(VarIdents, ArrIdents, Instr)
% State representation: state(Vars, Arrs, INrs)
% Vars: Map Ident Val, Arrs: Map Ident [Val]
% INrs: List of instruction numbers for each process
initState(program(Variables, Arrays, _), N, state(Vars, Arrs, INrs)) :-
   initVars(Variables, Vars),
   initArrs(Arrays, N, Arrs),
   initINrs(N, INrs).

% Initialize Vars with 0
initVars([], []).
initVars([Var|Variables], [Var-0|Vars]) :-
   initVars(Variables, Vars).

% Initialize Arrs with N arrays filled with 0
initArrs([], _, []).
initArrs([Arr|Arrays], N, [Arr-Array|Arrs]) :-
   initArray(N, 0, Array),
   initArrs(Arrays, N, Arrs).

% Initialize array of given size with given value
initArray(0, _, []).
initArray(Size, Val, [Val|T]) :-
   Size > 0,
   SizeMin is Size - 1,
   initArray(SizeMin, Val, T).

% Initialize INrs with 1
initINrs(0, []).
initINrs(N, [1|INrs]) :-
   N > 0,
   NMin is N - 1,
   initINrs(NMin, INrs).

% mapVal(+Pairs, +Key, -Value)
% Retrieve the Value associated with the given Key.
mapVal([Key-Value|_], Key, Value).
mapVal([_|Pairs], Key, Value) :-
   mapVal(Pairs, Key, Value).

% mapPutKey(+Pairs, +Key, +Value, -UpdatedPairs)
% Set the Value associated with the given Key.
mapPutKey([Key-_|Tail], Key, Value, [Key-Value|Tail]).
mapPutKey([Head|Tail], Key, Value, [Head|UpdatedTail]) :-
   mapPutKey(Tail, Key, Value, UpdatedTail).

% arrPutPos(+List, +Index, +Value, -NewList)
% Replace the element at the Index in the List with the given Value.
arrPutPos([_|Tail], 0, Value, [Value|Tail]).
arrPutPos([Head|Tail], Index, Value, [Head|NewTail]) :-
   Index > 0,
   NextIndex is Index - 1,
   arrPutPos(Tail, NextIndex, Value, NewTail).

% getINr(+State, +PrId, -INr)
% Get the instruction number of the given PrId.
getINr(state(_, _, INrs), PrId, INr) :-
   nth0(PrId, INrs, INr).

% step(+Prog, +StateIn, ?PrId, -StateOut)
% Execute process (PrId) instruction.
step(program(_, _, Instr), StateIn, PrId, StateOut) :-
   getINr(StateIn, PrId, INr),
   nth1(INr, Instr, Stmt),
   evS(Stmt, StateIn, PrId, StateOut).

% evS(+Stmt, +StateIn, +PrId, -StateOut) - evaluate statement
evS(assign(Var, ArrExpr), StateIn, PrId, StateOut) :-
   evArr(ArrExpr, StateIn, PrId, Val),
   setVar(StateIn, Var, Val, PrId, MidState),
   incINr(MidState, PrId, StateOut).

evS(goto(Index), StateIn, PrId, StateOut) :-
   setINr(StateIn, PrId, Index, StateOut).

evS(condGoto(BExpr, Index), StateIn, PrId, StateOut) :-
   evBool(BExpr, StateIn, PrId) ->
      setINr(StateIn, PrId, Index, StateOut)
   ;  incINr(StateIn, PrId, StateOut).

evS(sekcja, StateIn, PrId, StateOut) :-
   incINr(StateIn, PrId, StateOut).

% setVar(+StateIn, +Ident, +Val, +PrId, -StateOut)
setVar(state(Vars, Arrs, INrs), Ident, Val, _, state(UpVars, Arrs, INrs)) :-
   mapPutKey(Vars, Ident, Val, UpVars).

setVar(StateIn, array(Ident, ArrExpr), Val, PrId, StateOut) :-
   getArr(StateIn, Ident, Arr),
   evArr(ArrExpr, StateIn, PrId, Index),
   arrPutPos(Arr, Index, Val, UpArr),
   setArr(StateIn, Ident, UpArr, StateOut).

% getArr(+State, +Ident, -Arr)
getArr(state(_, Arrs, _), Ident, Arr) :-
   mapVal(Arrs, Ident, Arr).

% setArr(+StateIn, +Ident, +Arr, -StateOut)
setArr(state(Vars, Arrs, INrs), Ident, Arr, state(Vars, UpArrs, INrs)) :-
   mapPutKey(Arrs, Ident, Arr, UpArrs).

evBool(BExpr, State, PrId) :-
   BExpr =.. [Op, LeftExpr, RightExpr],
   evE(LeftExpr, State, PrId, LeftVal),
   evE(RightExpr, State, PrId, RightVal),
   call(Op, LeftVal, RightVal).

% evE(+Expr, +State, +PrId, -Val) - evaluate expression
evE(pid, _, PrId, PrId).

evE(Num, _, _, Num) :-
   number(Num).

evE(Var, State, PrId, Val) :-
   evVar(Var, State, PrId, Val).

evVar(Ident, state(Vars, _, _), _, Val) :-
   mapVal(Vars, Ident, Val).

evVar(array(Ident, ArrExpr), State, PrId, Val) :-
   getArr(State, Ident, Arr),
   evArr(ArrExpr, State, PrId, Index),
   nth0(Index, Arr, Val).

% evArr(+Expr, +State, +PrId, -Val)
evArr(Expr, State, PrId, Val) :-
   evE(Expr, State, PrId, Val).

evArr(ArrExpr, State, PrId, Val) :-
   ArrExpr =.. [Op, LeftExpr, RightExpr],
   evE(LeftExpr, State, PrId, LeftVal),
   evE(RightExpr, State, PrId, RightVal),
   Eval =.. [Op, LeftVal, RightVal],
   Val is Eval.

% setINr(+State, +PrId, +INr, -StateOut)
% Set the instruction number of the given PrId to INr.
setINr(state(Vars, Arrs, INrs), PrId, INr, state(Vars, Arrs, INrIncs)) :-
   arrPutPos(INrs, PrId, INr, INrIncs).

% incINr(+StateIn, +PrId, -StateOut)
% Increment the instruction number of a given PrId by one. 
incINr(StateIn, PrId, StateOut) :-
   getINr(StateIn, PrId, INr),
   INrInc is INr + 1,
   setINr(StateIn, PrId, INrInc, StateOut).

% generateSteps/3(+Prog, +State, -IsSafe)
% Checks whether the program is safe or not.
generateSteps(Prog, State, IsSafe) :-
   generateSteps(Prog, State, [], _, IsSafe).

% generateSteps/5(+Prog, +State +VisitedIn, -VisitedOut, -IsSafe)
% VisitedIn, VisitedOut - lists of visited states before and after execution.
generateSteps(Prog, State, VisitedIn, VisitedOut, IsSafe) :-
   (  member(State, VisitedIn) ->
      VisitedOut = VisitedIn,
      IsSafe = safe
   ;  (  unsafe(Prog, State, Procs) ->
         IsSafe = unsafe([], Procs)
      ;  getNStates(Prog, State, NStates),
         updateStates(Prog, State, NStates, [State|VisitedIn], VisitedOut, IsSafe)
      )
   ).

% updateStates(+Prog, +State, +NStates, +VisitedIn, ?VisitedOut, -IsSafe)
% Invoke all the reachable states of given state and check if all are safe.
updateStates(_, _, [], Visited, Visited, safe).
updateStates(Prog, State, [PrId-NextState|T], VisitedIn, VisitedOut, IsSafe) :-
   generateSteps(Prog, NextState, VisitedIn, VisitedMid, IsSafeM),
   (  IsSafeM = unsafe(Steps, Procs) ->
      getINr(State, PrId, INr),
      IsSafe = unsafe([PrId-INr|Steps], Procs)
   ; updateStates(Prog, State, T, VisitedMid, VisitedOut, IsSafe)
   ).

% getNStates(+Prog, +State, -NStates)
% Get all stats reachable from the given state.
getNStates(Prog, State, NStates) :-
   getNStates(Prog, State, [], NStates).

getNStates(Prog, State, Acc, NStates) :-
   % Generate a new neighboring state by making a step for each process.
   step(Prog, State, PrId, NextState),
   \+ member(PrId-_, Acc), % Ensure the new state is not visited.
   !,
   getNStates(Prog, State, [PrId-NextState|Acc], NStates).

getNStates(_, _, Acc, Acc). % Base case: When there are no more unvisited.

% unsafe(+Program, +State, -Procs) 
% Check if more than one process is in critical section in the given state.
% Procs - list of processes in critical section (sekcja).
unsafe(program(_, _, Instr), state(_, _, INrs), Procs) :-
   elemsOnPos(Instr, INrs, CurrentInstr),
   getAllInCricitS(sekcja, CurrentInstr, Procs),
   length(Procs, Count),
   Count > 1.

% elemsOnPos(+List, +Indices, -Elements)
% Retrieve elements from List at positions specified by Indices.
elemsOnPos(_, [], []).
elemsOnPos(List, [I|Indices], [E|Elements]) :-
   nth1(I, List, E),
   elemsOnPos(List, Indices, Elements).

% getAllInCricitS(+Element, +List, -Indices)
% Retrieves the indices of occurrences of Element in List.
getAllInCricitS(Element, List, Indices) :-
   getAllInCricitS(Element, List, 0, Indices).

getAllInCricitS(_, [], _, []).
getAllInCricitS(Element, [Element|Rest], Index, [Index|Indices]) :-
   NextIndex is Index + 1,
   getAllInCricitS(Element, Rest, NextIndex, Indices).
getAllInCricitS(Element, [_|Rest], Index, Indices) :-
   NextIndex is Index + 1,
   getAllInCricitS(Element, Rest, NextIndex, Indices).
