simple PyTic9
{
    parameters:
        @display("i=block/routing");
    gates:
        input in;
        output out;
}

simple PyToc9
{
    parameters:
        @display("i=block/process");
    gates:
        input in;
        output out;
}

//
// Same as Tictoc8.
//
network Tictoc9
{
    submodules:
        tic: PyTic9 {
            parameters:
                @display("i=,cyan");
        }
        toc: PyToc9 {
            parameters:
                @display("i=,gold");
        }
    connections:
        tic.out --> {  delay = 100ms; } --> toc.in;
        tic.in <-- {  delay = 100ms; } <-- toc.out;
}
