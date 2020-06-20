#include <pybind11/pybind11.h>

#include <omnetpp.h>
#include <omnetpp/ccomponent.h>

/*
 * make public all the protected members of cComponent
 * we want to expose to python
 */
class cComponentPublicist : public omnetpp::cComponent {
public:

    using omnetpp::cComponent::initialize;
    using omnetpp::cComponent::numInitStages;
    using omnetpp::cComponent::finish;
    using omnetpp::cComponent::handleParameterChange;
    using omnetpp::cComponent::refreshDisplay;
};


void bind_cComponent(pybind11::module &m)
{
    pybind11::class_<omnetpp::cComponent> py_cComponent(
        m,
        "_cComponent",
        R"docstring(
        `cComponent` provides parameters, properties, display string, RNG mapping,
        initialization and finalization support, simulation signals support,
        and several other services to its subclasses.

        Initialize and finish functions may be provided by the user,
        to perform special tasks at the beginning and the end of the simulation.
        The functions are made protected because they are supposed
        to be called only via `callInitialize()` and `callFinish()`.

        The initialization process was designed to support multi-stage
        initialization of compound modules (i.e. initialization in several
        'waves'). (Calling the `initialize()` function of a simple module is
        hence a special case). The initialization process is performed
        on a module like this. First, the number of necessary initialization
        stages is determined by calling `numInitStages()`, then `initialize(stage)`
        is called with `0,1,...numstages-1` as argument. The default
        implementation of `numInitStages()` and `initialize(stage)` provided here
        defaults to single-stage initialization, that is, `numInitStages()`
        returns 1 and initialize(stage) simply calls initialize() if stage is 0.
        )docstring"
    );

    pybind11::enum_<omnetpp::cComponent::ComponentKind>(py_cComponent, "ComponentKind")
        .value("KIND_MODULE", omnetpp::cComponent::ComponentKind::KIND_MODULE)
        .value("KIND_CHANNEL", omnetpp::cComponent::ComponentKind::KIND_CHANNEL)
        .value("KIND_OTHER", omnetpp::cComponent::ComponentKind::KIND_OTHER)
        .export_values();

    /*
     * cComponent is abstract and we don't need to instantiate it from python
     * so do not add init method.
     *

    py_cComponent.def(
        pybind11::init<const char*>(),
        R"docstring(
        Constructor. Note that module and channel objects should not be created
        directly, via their `cComponentType` objects. `cComponentType.create()`
        will do all housekeeping associated with creating the module (assigning
        an ID to the module, inserting it into the `simulation` object,
        etc.).
        )docstring",
        pybind11::arg("name") = nullptr
    );
    */

    py_cComponent.def(
        "initialize",
        pybind11::overload_cast<>(&cComponentPublicist::initialize),
        R"docstring(
        Single-stage initialization hook. This default implementation
        does nothing.
        )docstring"
    );

    py_cComponent.def(
        "initialize",
        pybind11::overload_cast<int>(&cComponentPublicist::initialize),
        R"docstring(
        Multi-stage initialization hook. This default implementation does
        single-stage init, that is, calls initialize() if stage is 0.
        )docstring",
        pybind11::arg("stage")
    );

    py_cComponent.def(
        "numInitStages",
        &cComponentPublicist::numInitStages,
        R"docstring(
        Multi-stage initialization hook, should be redefined to return the
        number of initialization stages required. This default implementation
        does single-stage init, that is, returns 1.
        )docstring"
    );

    py_cComponent.def(
        "finish",
        &cComponentPublicist::finish,
        R"docstring(
        Finish hook. `finish()` is called after end of simulation if it
        terminated without error. This default implementation does nothing.
        )docstring"
    );

    py_cComponent.def(
        "handleParameterChange",
        &cComponentPublicist::handleParameterChange,
        R"docstring(
        This method is called by the simulation kernel to notify the module or
        channel that the value of an existing parameter has changed. Redefining
        this method allows simple modules and channels to be react on parameter
        changes, for example by re-reading the value. This default implementation
        does nothing.

        The parameter name is `None` if more than one parameter has changed.

        To make it easier to write predictable components, the function is NOT
        called on uninitialized components (i.e. when `initialized()` returns
        `False`). For each component, the function is called (with `None` as a
        parname) after the last stage of the initialization, so that the module
        gets a chance to update its cached parameters.

        Also, one must be extremely careful when changing parameters from inside
        handleParameterChange(), to avoid creating an infinite notification loop.
        )docstring",
        pybind11::arg("parname")
    );

    py_cComponent.def(
        "refreshDisplay",
        &cComponentPublicist::refreshDisplay,
        R"docstring(
        This method is called on all components of the simulation by graphical
        user interfaces (Qtenv, Tkenv) whenever GUI contents need to be refreshed
        after processing some simulation events. Components that contain
        visualization-related code are expected to override `refreshDisplay()`,
        and move visualization code display string manipulation, canvas figures
        maintenance, OSG scene graph update, etc.) into it.

        As it is unpredictable when and whether this method is invoked, the
        simulation logic should not depend on it. It is advisable that code in
        `refreshDisplay()` does not alter the state of the model at all. This
        behavior is gently encouraged by having this method declared as const.
        (Data members that do need to be updated inside refreshDisplay(), i.e.
        those related to visualization, may be declared mutable to allow that).

        Tkenv and Qtenv invoke `refreshDisplay()` with similar strategies: in Step
        and Run mode, after each event; in Fast Run and Express Run mode, every
        time the screen is refereshed, which is typically on the order of once
        per second. Cmdenv does not invoke `refreshDisplay()` at all.

        Note that overriding `refreshDisplay()` is generally preferable to doing
        display updates as part of event handling: it results in essentially
        zero per-event runtime overhead, and potentially more consistent
        information being displayed (as all components refresh their visualization,
        not only the one which has just processed an event.)
        )docstring"
    );

    py_cComponent.def(
        "getId",
        &omnetpp::cComponent::getId,
        R"docstring(
        Returns the component's ID in the simulation object (cSimulation).
        Component IDs are guaranteed to be unique during a simulation run
        (that is, IDs of deleted components are not reused for new components.)
        )docstring"
    );

    py_cComponent.def(
        "getNedTypeName",
        &omnetpp::cComponent::getNedTypeName,
        R"docstring(
        Returns the fully qualified NED type name of the component (i.e. the
        simple name prefixed with the package name and any existing enclosing
        NED type names).

        This method is a shortcut to `self.getComponentType().getFullName()`.
        )docstring"
    );

    py_cComponent.def(
        "isModule",
        &omnetpp::cComponent::isModule,
        R"docstring(
        Returns true for cModule and subclasses, otherwise false.
        )docstring"
    );

    py_cComponent.def(
        "isChannel",
        &omnetpp::cComponent::isChannel,
        R"docstring(
        Returns true for channels, and false otherwise.
        )docstring"
    );

    py_cComponent.def(
        "getSimulation",
        &omnetpp::cComponent::getSimulation,
        R"docstring(
        Returns the simulation the component is part of. Currently may only
        be invoked if the component's simulation is the active one (see
        cSimulation::getActiveSimulation()).
        )docstring",
        pybind11::return_value_policy::reference
    );

    py_cComponent.def(
        "getSystemModule",
        &omnetpp::cComponent::getSystemModule,
        R"docstring(
        Returns the toplevel module in the current simulation.
        This is a shortcut to `self.getSimulation().getSystemModule()`.
        )docstring"
    );

    py_cComponent.def(
        "getNumParams",
        &omnetpp::cComponent::getNumParams,
        R"docstring(
        Returns total number of the component's parameters.
        )docstring"
    );

    py_cComponent.def(
        "par",
        pybind11::overload_cast<int>(&omnetpp::cComponent::par),
        R"docstring(
        Returns reference to the parameter identified with its
        index k. Throws an error if the parameter does not exist.
        )docstring",
        pybind11::arg("k"),
        pybind11::return_value_policy::reference
    );

    py_cComponent.def(
        "par",
        pybind11::overload_cast<const char *>(&omnetpp::cComponent::par),
        R"docstring(
        Returns reference to the parameter specified with its name.
        Throws an error if the parameter does not exist.
        )docstring",
        pybind11::arg("parname"),
        pybind11::return_value_policy::reference
    );

    py_cComponent.def(
        "findPar",
        &omnetpp::cComponent::findPar,
        R"docstring(
        Returns index of the parameter specified with its name.
        Returns -1 if the object doesn't exist.
        )docstring",
        pybind11::arg("parname")
    );

    py_cComponent.def(
        "hasPar",
        &omnetpp::cComponent::hasPar,
        R"docstring(
        Check if a parameter exists.
        )docstring",
        pybind11::arg("parname")
    );

    py_cComponent.def(
        "getRNG",
        &omnetpp::cComponent::getRNG,
        R"docstring(
        Returns the global RNG mapped to local RNG number k. For large indices
        (k >= map size) the global RNG k is returned, provided it exists.
        )docstring",
        pybind11::arg("k")
    );

    py_cComponent.def(
        "intrand",
        &omnetpp::cComponent::intrand,
        R"docstring(
        Produces a random integer in the range [0,r) using the RNG given with its index.
        )docstring",
        pybind11::arg("r"), pybind11::arg("rng") = 0
    );

    py_cComponent.def(
        "dblrand",
        &omnetpp::cComponent::dblrand,
        R"docstring(
        Produces a random double in the range [0,1) using the RNG given with its index.
        )docstring",
        pybind11::arg("rng") = 0
    );

    py_cComponent.def(
        "uniform",
        pybind11::overload_cast<double, double, int>(&omnetpp::cComponent::uniform, pybind11::const_),
        R"docstring(
        Returns a random variate with uniform distribution in the range [a,b).

        :param a, b: the interval, a < b
        :param rng: index of the component RNG to use, see `getRNG(int)`
        )docstring",
        pybind11::arg("a"), pybind11::arg("a"), pybind11::arg("rng") = 0
    );

    py_cComponent.def(
        "uniform",
        pybind11::overload_cast<omnetpp::SimTime, omnetpp::SimTime, int>(&omnetpp::cComponent::uniform, pybind11::const_),
        R"docstring(
        SimTime version of uniform(double, double, int), for convenience.
        )docstring",
        pybind11::arg("a"), pybind11::arg("a"), pybind11::arg("rng") = 0
    );

    py_cComponent.def(
        "exponential",
        pybind11::overload_cast<double, int>(&omnetpp::cComponent::exponential, pybind11::const_),
        R"docstring(
        Returns a random variate from the exponential distribution with the
        given mean (that is, with parameter lambda=1/mean).

        :param mean: mean value
        :param rng: index of the component RNG to use, see getRNG(int)
        )docstring",
        pybind11::arg("mean"), pybind11::arg("rng") = 0
    );

    py_cComponent.def(
        "exponential",
        pybind11::overload_cast<omnetpp::SimTime, int>(&omnetpp::cComponent::exponential, pybind11::const_),
        R"docstring(
        SimTime version of exponential(double,int), for convenience.
        )docstring",
        pybind11::arg("mean"), pybind11::arg("rng") = 0
    );

    py_cComponent.def(
        "normal",
        pybind11::overload_cast<double, double, int>(&omnetpp::cComponent::normal, pybind11::const_),
        R"docstring(
        Returns a random variate from the normal distribution with the given mean
        and standard deviation.

        :param mean: mean of the normal distribution
        :param stddev: standard deviation of the normal distribution
        :param rng: index of the component RNG to use, see getRNG(int)
        )docstring",
        pybind11::arg("mean"), pybind11::arg("stddev"), pybind11::arg("rng") = 0
    );

    py_cComponent.def(
        "normal",
        pybind11::overload_cast<omnetpp::SimTime, omnetpp::SimTime, int>(&omnetpp::cComponent::normal, pybind11::const_),
        R"docstring(
        SimTime version of normal(double, double, int), for convenience.
        )docstring",
        pybind11::arg("mean"), pybind11::arg("stddev"), pybind11::arg("rng") = 0
    );

    py_cComponent.def(
        "truncnormal",
        pybind11::overload_cast<double, double, int>(&omnetpp::cComponent::truncnormal, pybind11::const_),
        R"docstring(
        Normal distribution truncated to nonnegative values.
        It is implemented with a loop that discards negative values until
        a nonnegative one comes. This means that the execution time is not bounded:
        a large negative mean with much smaller stddev is likely to result
        in a large number of iterations.

        The mean and stddev parameters serve as parameters to the normal
        distribution <i>before</i> truncation. The actual random variate returned
        will have a different mean and standard deviation.

        :param mean: mean of the normal distribution
        :param stddev: standard deviation of the normal distribution
        :param rng: index of the component RNG to use, see getRNG(int)
        )docstring",
        pybind11::arg("mean"), pybind11::arg("stddev"), pybind11::arg("rng") = 0
    );

    py_cComponent.def(
        "truncnormal",
        pybind11::overload_cast<omnetpp::SimTime, omnetpp::SimTime, int>(&omnetpp::cComponent::truncnormal, pybind11::const_),
        R"docstring(
        SimTime version of truncnormal(double,double,int), for convenience.
        )docstring",
        pybind11::arg("mean"), pybind11::arg("stddev"), pybind11::arg("rng") = 0
    );

    py_cComponent.def(
        "gamma_d",
        &omnetpp::cComponent::gamma_d,
        R"docstring(
        Returns a random variate from the gamma distribution with parameters
        alpha>0, theta>0. Alpha is known as the "shape" parameter, and theta
        as the "scale" parameter.

        Some sources in the literature use the inverse scale parameter
        beta = 1 / theta, called the "rate" parameter. Various other notations
        can be found in the literature; our usage of (alpha,theta) is consistent
        with Wikipedia and Mathematica (Wolfram Research).

        Gamma is the generalization of the Erlang distribution for non-integer
        k values, which becomes the alpha parameter. The chi-square distribution
        is a special case of the gamma distribution.

        For alpha=1, Gamma becomes the exponential distribution with mean=theta.

        The mean of this distribution is alpha*theta, and variance is alpha*theta<sup>2</sup>.

        Generation: if alpha=1, it is generated as exponential(theta).

        For alpha>1, we make use of the acceptance-rejection method in
        "A Simple Method for Generating Gamma Variables", George Marsaglia and
        Wai Wan Tsang, ACM Transactions on Mathematical Software, Vol. 26, No. 3,
        September 2000.

        The alpha < 1 case makes use of the alpha > 1 algorithm, as suggested by the
        above paper.

        .. note::

            The name gamma_d is chosen to avoid ambiguity with a function of the same name

        :param alpha: >0  the "shape" parameter
        :param theta: >0  the "scale" parameter
        :param rng: index of the component RNG to use, see getRNG(int)
        )docstring",
        pybind11::arg("alpha"), pybind11::arg("theta"), pybind11::arg("rng") = 0
    );

    py_cComponent.def(
        "beta",
        &omnetpp::cComponent::beta,
        R"docstring(
        Returns a random variate from the beta distribution with parameters
        alpha1, alpha2.

        Generation is using relationship to Gamma distribution: if Y1 has gamma
        distribution with alpha=alpha1 and beta=1 and Y2 has gamma distribution
        with alpha=alpha2 and beta=2, then Y = Y1/(Y1+Y2) has beta distribution
        with parameters alpha1 and alpha2.

        :param alpha1, alpha2: >0
        :param rng :index of the component RNG to use, see getRNG(int)
        )docstring",
        pybind11::arg("alpha1"), pybind11::arg("alpha2"), pybind11::arg("rng") = 0
    );

    py_cComponent.def(
        "erlang_k",
        &omnetpp::cComponent::erlang_k,
        R"docstring(
        Returns a random variate from the Erlang distribution with k phases
        and mean mean.

        This is the sum of k mutually independent random variables, each with
        exponential distribution. Thus, the kth arrival time
        in the Poisson process follows the Erlang distribution.

        Erlang with parameters m and k is gamma-distributed with alpha=k
        and beta=m/k.

        Generation makes use of the fact that exponential distributions
        sum up to Erlang.

        :param k: number of phases, k>0
        :param mean: >0
        :@param rng: index of the component RNG to use, see getRNG(int)
        )docstring",
        pybind11::arg("k"), pybind11::arg("mean"), pybind11::arg("rng") = 0
    );

    py_cComponent.def(
        "chi_square",
        &omnetpp::cComponent::chi_square,
        R"docstring(

        Returns a random variate from the chi-square distribution
        with k degrees of freedom.  The chi-square distribution arises
        in statistics. If Yi are k independent random variates from the normal
        distribution with unit variance, then the sum-of-squares (sum(Yi^2))
        has a chi-square distribution with k degrees of freedom.

        The expected value of this distribution is k. Chi_square with parameter
        k is gamma-distributed with alpha=k/2, beta=2.

        Generation is using relationship to gamma distribution.

        :param k: degrees of freedom, k>0
        :param rng: index of the component RNG to use, see getRNG(int)
        )docstring",
        pybind11::arg("k"), pybind11::arg("rng") = 0
    );

    py_cComponent.def(
        "student_t",
        &omnetpp::cComponent::student_t,
        R"docstring(
        Returns a random variate from the student-t distribution with
        i degrees of freedom. If Y1 has a normal distribution and Y2 has a chi-square
        distribution with k degrees of freedom then X = Y1 / sqrt(Y2/k)
        has a student-t distribution with k degrees of freedom.

        Generation is using relationship to gamma and chi-square.

        :param i: degrees of freedom, i>0
        :param rng: index of the component RNG to use, see getRNG(int)
        )docstring",
        pybind11::arg("i"), pybind11::arg("rng") = 0
    );

    py_cComponent.def(
        "cauchy",
        &omnetpp::cComponent::cauchy,
        R"docstring(

        Returns a random variate from the Cauchy distribution (also called
        Lorentzian distribution) with parameters a,b where b>0.

        This is a continuous distribution describing resonance behavior.
        It also describes the distribution of horizontal distances at which
        a line segment tilted at a random angle cuts the x-axis.

        Generation uses inverse transform.

        :param a:
        :param b:  b>0
        :param rng: index of the component RNG to use, see getRNG(int)
        )docstring",
        pybind11::arg("a"), pybind11::arg("b"), pybind11::arg("rng") = 0
    );

    py_cComponent.def(
        "triang",
        &omnetpp::cComponent::triang,
        R"docstring(
        Returns a random variate from the triangular distribution with parameters
        a <= b <= c.

        Generation uses inverse transform.

        :param a, b, c:   a <= b <= c
        :param rng: index of the component RNG to use, see getRNG(int)
        )docstring",
        pybind11::arg("a"), pybind11::arg("b"), pybind11::arg("c"), pybind11::arg("rng") = 0
    );

    py_cComponent.def(
        "lognormal",
        &omnetpp::cComponent::lognormal,
        R"docstring(
        Returns a random variate from the lognormal distribution with "scale"
        parameter m and "shape" parameter w. m and w correspond to the parameters
        of the underlying normal distribution (m: mean, w: standard deviation.)

        Generation is using relationship to normal distribution.

        :param m:  "scale" parameter, m>0
        :param w:  "shape" parameter, w>0
        :param rng: index of the component RNG to use, see getRNG(int)
        )docstring",
        pybind11::arg("m"), pybind11::arg("w"), pybind11::arg("rng") = 0
    );

    py_cComponent.def(
        "weibull",
        &omnetpp::cComponent::weibull,
        R"docstring(
        Returns a random variate from the Weibull distribution with parameters
        a, b > 0, where a is the "scale" parameter and b is the "shape" parameter.
        Sometimes Weibull is given with alpha and beta parameters, then alpha=b
        and beta=a.

        The Weibull distribution gives the distribution of lifetimes of objects.
        It was originally proposed to quantify fatigue data, but it is also used
        in reliability analysis of systems involving a "weakest link," e.g.
        in calculating a device's mean time to failure.

        When b=1, Weibull(a,b) is exponential with mean a.

        Generation uses inverse transform.

        :param a:  the "scale" parameter, a>0
        :param b:  the "shape" parameter, b>0
        :param rng: index of the component RNG to use, see getRNG(int)
        )docstring",
        pybind11::arg("a"), pybind11::arg("b"), pybind11::arg("rng") = 0
    );

    py_cComponent.def(
        "pareto_shifted",
        &omnetpp::cComponent::pareto_shifted,
        R"docstring(
        Returns a random variate from the shifted generalized Pareto distribution.

        Generation uses inverse transform.

        :param a, b:  the usual parameters for generalized Pareto
        :param c:   shift parameter for left-shift
        :param rng: index of the component RNG to use, see getRNG(int)
        )docstring",
        pybind11::arg("a"), pybind11::arg("b"), pybind11::arg("c"), pybind11::arg("rng") = 0
    );

    py_cComponent.def(
        "intuniform",
        &omnetpp::cComponent::intuniform,
        R"docstring(
        Returns a random integer with uniform distribution in the range [a,b],
        inclusive. (Note that the function can also return b.)

        :param a, b:  the interval, a<b
        :param rng: index of the component RNG to use, see getRNG(int)
        )docstring",
        pybind11::arg("a"), pybind11::arg("b"), pybind11::arg("rng") = 0
    );

    py_cComponent.def(
        "intuniformexcl",
        &omnetpp::cComponent::intuniformexcl,
        R"docstring(
        Returns a random integer with uniform distribution over [a,b), that is,
        from [a,b-1].

        :param a, b:  the interval, a<b
        :param rng: index of the component RNG to use, see getRNG(int)
        )docstring",
        pybind11::arg("a"), pybind11::arg("b"), pybind11::arg("rng") = 0
    );

    py_cComponent.def(
        "bernoulli",
        &omnetpp::cComponent::bernoulli,
        R"docstring(
        Returns the result of a Bernoulli trial with probability p,
        that is, 1 with probability p and 0 with probability (1-p).

        Generation is using elementary look-up.

        :param p:  0=<p<=1
        :param rng: index of the component RNG to use, see getRNG(int)
        )docstring",
        pybind11::arg("p"), pybind11::arg("rng") = 0
    );

    py_cComponent.def(
        "binomial",
        &omnetpp::cComponent::binomial,
        R"docstring(
        Returns a random integer from the binomial distribution with
        parameters n and p, that is, the number of successes in n independent
        trials with probability p.

        Generation is using the relationship to Bernoulli distribution (runtime
        is proportional to n).

        :param n: n>=0
        :param p: 0<=p<=1
        :param rng: index of the component RNG to use, see getRNG(int)
        )docstring",
        pybind11::arg("n"), pybind11::arg("p"), pybind11::arg("rng") = 0
    );

    py_cComponent.def(
        "geometric",
        &omnetpp::cComponent::geometric,
        R"docstring(
        Returns a random integer from the geometric distribution with parameter p,
        that is, the number of independent trials with probability p until the
        first success.

        This is the n=1 special case of the negative binomial distribution.

        Generation uses inverse transform.

        :param p:  0<p<=1
        :param rng: index of the component RNG to use, see getRNG(int)
        )docstring",
        pybind11::arg("p"), pybind11::arg("rng") = 0
    );

    py_cComponent.def(
        "negbinomial",
        &omnetpp::cComponent::negbinomial,
        R"docstring(
        Returns a random integer from the negative binomial distribution with
        parameters n and p, that is, the number of failures occurring before
        n successes in independent trials with probability p of success.

        Generation is using the relationship to geometric distribution (runtime is
        proportional to n).

        :param n:  n>=0
        :param p:  0<p<1
        :param rng: index of the component RNG to use, see getRNG(int)
        )docstring",
        pybind11::arg("n"), pybind11::arg("p"), pybind11::arg("rng") = 0
    );

    py_cComponent.def(
        "poisson",
        &omnetpp::cComponent::poisson,
        R"docstring(
        Returns a random integer from the Poisson distribution with parameter lambda,
        that is, the number of arrivals over unit time where the time between
        successive arrivals follow exponential distribution with parameter lambda.

        Lambda is also the mean (and variance) of the distribution.

        Generation method depends on value of lambda:

          - 0<lambda<=30: count number of events
          - lambda>30: Acceptance-Rejection due to Atkinson (see Banks, page 166)

        :param lambda:  > 0
        :param rng: index of the component RNG to use, see getRNG(int)
        )docstring",
        pybind11::arg("lambda"), pybind11::arg("rng") = 0
    );

    py_cComponent.def_static(
        "registerSignal",
        &omnetpp::cComponent::registerSignal,
        R"docstring(
        Returns the signal ID (handle) for the given signal name. Signal names
        and IDs are global. The signal ID for a particular name gets assigned
        at the first registerSignal() call; further registerSignal() calls for
        the same name will return the same ID.
        )docstring",
        pybind11::arg("name")
    );

    py_cComponent.def_static(
        "getSignalName",
        &omnetpp::cComponent::getSignalName,
        R"docstring(
        The inverse of `registerSignal()`: returns the name of the given signal,
        or `None` for invalid signal handles.
        )docstring",
        pybind11::arg("signalID")
    );

    py_cComponent.def(
        "emit",
        pybind11::overload_cast<omnetpp::simsignal_t, long, omnetpp::cObject *>(&omnetpp::cComponent::emit),
        R"docstring(
        Emits the long value as a signal. If the given signal has listeners in this
        component or in ancestor components, their appropriate receiveSignal() methods
        are called. If there are no listeners, the runtime cost is usually minimal.
        )docstring",
        pybind11::arg("signalID"), pybind11::arg("l"), pybind11::arg("details") = nullptr
    );

    py_cComponent.def(
        "emit",
        pybind11::overload_cast<omnetpp::simsignal_t, double, omnetpp::cObject *>(&omnetpp::cComponent::emit),
        R"docstring(
        Emits the double value as a signal. If the given signal has listeners in this
        component or in ancestor components, their appropriate receiveSignal() methods
        are called. If there are no listeners, the runtime cost is usually minimal.
        )docstring",
        pybind11::arg("signalID"), pybind11::arg("d"), pybind11::arg("details") = nullptr);

    py_cComponent.def(
        "emit",
        pybind11::overload_cast<omnetpp::simsignal_t, const omnetpp::SimTime&, omnetpp::cObject *>(&omnetpp::cComponent::emit),
        R"docstring(
        Emits the simtime_t value as a signal. If the given signal has listeners in this
        component or in ancestor components, their appropriate receiveSignal() methods
        are called. If there are no listeners, the runtime cost is usually minimal.
        )docstring",
        pybind11::arg("signalID"), pybind11::arg("t"), pybind11::arg("details") = nullptr
    );

    py_cComponent.def(
        "emit",
        pybind11::overload_cast<omnetpp::simsignal_t, const char*, omnetpp::cObject *>(&omnetpp::cComponent::emit),
        R"docstring(
        Emits the given string as a signal. If the given signal has listeners in this
        component or in ancestor components, their appropriate receiveSignal() methods
        are called. If there are no listeners, the runtime cost is usually minimal.
        )docstring",
        pybind11::arg("signalID"), pybind11::arg("s"), pybind11::arg("details") = nullptr);

    py_cComponent.def(
        "emit",
        pybind11::overload_cast<omnetpp::simsignal_t, const omnetpp::cObject*, omnetpp::cObject *>(&omnetpp::cComponent::emit),
        R"docstring(
        Emits the given object as a signal. If the given signal has listeners in this
        component or in ancestor components, their appropriate receiveSignal() methods
        are called. If there are no listeners, the runtime cost is usually minimal.
        )docstring",
        pybind11::arg("signalID"), pybind11::arg("obj"), pybind11::arg("details") = nullptr
    );

    py_cComponent.def(
        "emit",
        pybind11::overload_cast<omnetpp::simsignal_t, bool, omnetpp::cObject *>(&omnetpp::cComponent::emit),
        R"docstring(
        Emits the boolean value as a signal. If the given signal has listeners in this
        component or in ancestor components, their appropriate receiveSignal() methods
        are called. If there are no listeners, the runtime cost is usually minimal.
        )docstring",
        pybind11::arg("signalID"), pybind11::arg("b"), pybind11::arg("details") = nullptr
    );

    py_cComponent.def(
        "mayHaveListeners",
        &omnetpp::cComponent::mayHaveListeners,
        R"docstring(
        If producing a value for a signal has a significant runtime cost, this
        method can be used to check beforehand whether the given signal possibly
        has any listeners at all. if not, emitting the signal can be skipped.
        This method has a constant cost but may return false positive.
        )docstring",
        pybind11::arg("signalID")
    );

    py_cComponent.def(
        "hasListeners",
        &omnetpp::cComponent::hasListeners,
        R"docstring(
        Returns true if the given signal has any listeners. In the current
        implementation, this involves checking listener lists in ancestor
        modules until the first listener is found, or up to the root.
        This method may be useful if producing the data for an emit()
        call would be expensive compared to a hasListeners() call.
        )docstring",
        pybind11::arg("signalID")
    );

    py_cComponent.def(
        "recordScalar",
        pybind11::overload_cast<const char *, double, const char *>(&omnetpp::cComponent::recordScalar),
        R"docstring(
        Records a double into the scalar result file.
        )docstring",
        pybind11::arg("name"), pybind11::arg("value"), pybind11::arg("unit") = nullptr
    );

    py_cComponent.def(
        "recordScalar",
        pybind11::overload_cast<const char *, omnetpp::simtime_t, const char *>(&omnetpp::cComponent::recordScalar),
        R"docstring(
        Convenience method, delegates to recordScalar(const char *, double, const char*).
        )docstring",
        pybind11::arg("name"), pybind11::arg("value"), pybind11::arg("unit") = nullptr
    );

    py_cComponent.def(
        "hasGUI",
        &omnetpp::cComponent::hasGUI,
        R"docstring(
        Returns true if the current environment is a graphical user interface.
        (For example, it returns true if the simulation is running over Tkenv/Qtenv,
        and false if it's running over Cmdenv.) Modules can examine this flag
        to decide whether or not they need to bother with visualization, e.g.
        dynamically update display strings or draw on canvases.

        This method delegates to cEnvir::isGUI().
        )docstring"
    );

    py_cComponent.def(
        "getDisplayString",
        &omnetpp::cComponent::getDisplayString,
        R"docstring(
        Returns the display string which defines presentation when the module
        is displayed as a submodule in a compound module graphics.
        )docstring",
        pybind11::return_value_policy::reference
    );

    py_cComponent.def(
        "setDisplayString",
        &omnetpp::cComponent::setDisplayString,
        R"docstring(
        Shortcut to `getDisplayString().set(dispstr)`
        )docstring",
        pybind11::arg("dispstr")
    );

    py_cComponent.def(
        "bubble",
        &omnetpp::cComponent::bubble,
        R"docstring(
        When the models is running under Tkenv or Qtenv, it displays the given
        text in the network graphics, as a bubble above the module's icon.
        )docstring",
        pybind11::arg("text")
    );

    py_cComponent.def(
        "recordStatistic",
        pybind11::overload_cast<omnetpp::cStatistic *, const char *>(&omnetpp::cComponent::recordStatistic),
        R"docstring(
        Records the given statistics into the scalar result file. Delegates to
        cStatistic::recordAs(). Note that if the statistics object is a histogram,
        this operation may invoke its transform() method.
        )docstring",
        pybind11::arg("stats"), pybind11::arg("unit") = nullptr
    );

    py_cComponent.def(
        "recordStatistic",
        pybind11::overload_cast<const char *, omnetpp::cStatistic *, const char *>(&omnetpp::cComponent::recordStatistic),
        R"docstring(
        Records the given statistics into the scalar result file. Delegates to
        cStatistic::recordAs(). Note that if the statistics object is a histogram,
        this operation may invoke its transform() method.
        )docstring",
        pybind11::arg("name"), pybind11::arg("stats"), pybind11::arg("unit") = nullptr
    );

}
