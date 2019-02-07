#include "unit_tests/anomalyDetectionUnitTestsImpl.h"
#include "unit_tests/densityCurveUnitTestsImpl.h"
#include "unit_tests/saxUnitTestsImpl.h"
#include "unit_tests/sequiturUnitTestsImpl.h"

int main(int argc, char *argv[])
{
    std::string in = "The Big Bang theory is the prevailing cosmological model \
    for the observable universe from the earliest known periods through \
    its subsequent large-scale evolution. The model describes \
    how the universe expanded from a very high-density and high-temperature state, \
    and offers a comprehensive explanation for a broad range of phenomena, including the \
    abundance of light elements, the cosmic microwave background (CMB), large scale structure and Hubble's law \
    (the farther away galaxies are, the faster they are moving away from Earth). If the observed conditions \
    are extrapolated backwards in time using the known laws of physics, the prediction is that just before a period \
    of very high density there was a singularity which is typically associated with the Big Bang. Physicists are \
    undecided whether this means the universe began from a singularity, or that current knowledge is insufficient \
    to describe the universe at that time. Detailed measurements of the expansion rate of the universe place the \
    Big Bang at around 13.8 billion years ago, which is thus considered the age of the universe. After its \
    initial expansion, the universe cooled sufficiently to allow the formation of subatomic particles, and later \
    simple atoms. Giant clouds of these primordial elements (mostly hydrogen, with some helium and lithium) \
    later coalesced through gravity, eventually forming early stars and galaxies, the descendants of which \
    are visible today. Astronomers also observe the gravitational effects of dark matter surrounding galaxies. \
    Though most of the mass in the universe seems to be in the form of dark matter, Big Bang theory and \
    various observations seem to indicate that it is not made out of conventional baryonic matter \
    (protons, neutrons, and electrons) but it is unclear exactly what it is made out of.";
    std::string out;
    sequiturStringCompressionUnitTest(in, out);

    if (in != out)
    {
        printf("\nSequitur error!\n");
    }
    else
    {
        printf("\nSequitur's compression OK!\n");
    }

    return 0;
}