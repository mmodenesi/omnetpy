simple PyTxc4
{
    parameters:
        bool sendMsgOnInit = default(false); // whether the module should send out a message on initialization
        int limit = default(2);   // another parameter with a default value
        @display("i=block/routing");
    gates:
        input in;
        output out;
}

//
// Adding module parameters.
//
network Tictoc4
{
    submodules:
        tic: PyTxc4 {
            parameters:
                sendMsgOnInit = true;
                @display("i=,cyan");
        }
        toc: PyTxc4 {
            parameters:
                sendMsgOnInit = false;
                @display("i=,gold");
        }
    connections:
        tic.out --> {  delay = 100ms; } --> toc.in;
        tic.in <-- {  delay = 100ms; } <-- toc.out;
}

