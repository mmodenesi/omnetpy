# More advanced usage

After completing the [getting started guide](../getstarted) you may want to start creating more
complex simulations. Take a look at the [pysamples directory](../pysamples). These simulations are
available on the image, as it is shown below:


```
you@yourmachine$ docker run \
    --rm -ti \
    -e DISPLAY=$DISPLAY \
    -v /tmp/.X11-unix:/tmp/.X11-unix mmodenesi/omnetpy:opp-6.0pre8 bash
```

On the container, navigate to some of the pysamples, compile and run:

```
userpp@7784e6f825d0 (omnetpp-dev):~/workspace$ cd ~/pysamples/pyrouting/
userpp@7784e6f825d0 (omnetpp-dev):~/pysamples/pyrouting$ make
modules.cc
Creating executable: out/gcc-release//pyrouting
userpp@7784e6f825d0 (omnetpp-dev):~/pysamples/pyrouting$ ./pyrouting
```
