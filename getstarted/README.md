# How to create a simulation using python

This guide will show you how to write the TicToc simulation from the [official
tutorial](https://docs.omnetpp.org/tutorials/tictoc/).

This guide was written and tested using OMNeT++ ``6.0pre8``.

### Prerequisites:

- docker
- linux environment, to launch containers in a graphical environment (only tested in ubuntu).


## Step 1: Launch a container


```
you@yourmachine$ docker run \
    --rm -ti \
    -e DISPLAY=$DISPLAY \
    -v /tmp/.X11-unix:/tmp/.X11-unix mmodenesi/omnetpy:opp-6.0pre8 bash
```

On the container, launch the omnetpp IDE:

```
userpp@container$ omnetpp
```


## Step 2: Prepare a new project

Select _File -> New -> OMNeT++ Project_

![new project](./img/new_project.png)
