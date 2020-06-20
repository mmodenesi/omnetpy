# omnetpy

[OMNeT++](https://omnetpp.org/) meets python

![pytictoc and its code](./pytictoc.png)

## What does this repository provide?

**TL;DR**: implement OMNeT++ simulations using python programming language:

- no need to know C++
- all the power of python is sudenly available
- faster prototypes
- even using the OMNeT++ IDE (if you wish...)

**Ok, I'm in... tell me more...**: TBD

## How can I use it?

Launch a contianer:

```
you@yourmachine$ docker run --rm -ti -e DISPLAY=$DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix mmodenesi/omnetpy bash
```

On the container, launch the omnetpp IDE:

```
userpp@container$ omnetpp
```

Follow the guides on how to create simulations with python (TBD)

## Who could be interested in this?

- You want to build simulations using OMNeT++ but your C++ knowledge is weak.
- You want to use OMNeT++ for educational purposes, and your students are more proficient in python than C++.
- You want to prototype a model faster, you can rewrite the code in C++ later.
- You are interested in making changes to the code and being able to use it without any extra compilation steps.

## What are the limitations?

TBD

## What is the current state of this project?
