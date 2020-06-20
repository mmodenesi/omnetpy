//
// Same as Txc4. This module will be the base of the Tic and Toc types.
//
simple PyTxc5
{
    parameters:
        bool sendMsgOnInit = default(false);
        int limit = default(2);
        @display("i=block/routing");
    gates:
        input in;
        output out;
}

//
// Specialize the module by defining parameters. We could have left the whole body
// empty, because the default value of the sendMsgOnInit parameter is false anyway.
// Note that the limit parameter is still unbound here.
//
simple PyTic5 extends PyTxc5
{
    parameters:
        @display("i=,cyan");
        sendMsgOnInit = true;   // Tic modules should send a message on init
}

//
// Specialize the module by defining parameters. We could have left the whole body
// empty, because the default value of the sendMsgOnInit parameter is false anyway.
// Note that the limit parameter is still unbound here.
//
simple PyToc5 extends PyTxc5
{
    parameters:
        @display("i=,gold");
        sendMsgOnInit = false;  // Toc modules should NOT send a message on init
}

//
// Adding module parameters.
//
network Tictoc5
{
    submodules:
        tic: PyTic5;  // the limit parameter is still unbound here. We will get it from the ini file
        toc: PyToc5;
    connections:
        tic.out --> {  delay = 100ms; } --> toc.in;
        tic.in <-- {  delay = 100ms; } <-- toc.out;
}
