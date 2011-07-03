// C headers
#include <unistd.h>

// C++ headers
#include <iostream>
using namespace std;

// Qt headers
#include <QCoreApplication>
#include <QEventLoop>

// libmyth headers
#include "exitcodes.h"
#include "mythcontext.h"
#include "mythdb.h"
#include "mythversion.h"
#include "util.h"
#include "mythtranslation.h"
#include "mythconfig.h"
#include "mythcommandlineparser.h"
#include "mythlogging.h"

#include "lookup.h"

class MPUBLIC MythMetadataLookupCommandLineParser : public MythCommandLineParser
{
  public:
    MythMetadataLookupCommandLineParser();
    void LoadArguments(void);
};

MythMetadataLookupCommandLineParser::MythMetadataLookupCommandLineParser() :
    MythCommandLineParser("mythmetadatalookup")
{ LoadArguments(); }

void MythMetadataLookupCommandLineParser::LoadArguments(void)
{
    addHelp();
    addVersion();
    addVerbose();
    addRecording();
    addLogging();

    add("--refresh-all", "refresh-all", false,
            "Refresh all recorded programs and recording rules metadata", "");
}

int main(int argc, char *argv[])
{
    MythMetadataLookupCommandLineParser cmdline;
    if (!cmdline.Parse(argc, argv))
    {
        cmdline.PrintHelp();
        return GENERIC_EXIT_INVALID_CMDLINE;
    }

    if (cmdline.toBool("showhelp"))
    {
        cmdline.PrintHelp();
        return GENERIC_EXIT_OK;
    }

    if (cmdline.toBool("showversion"))
    {
        cmdline.PrintVersion();
        return GENERIC_EXIT_OK;
    }

    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName("mythmetadatalookup");

    int retval;
    if ((retval = cmdline.ConfigureLogging()) != GENERIC_EXIT_OK)
        return retval;

    ///////////////////////////////////////////////////////////////////////
    // Don't listen to console input
    close(0);

    gContext = new MythContext(MYTH_BINARY_VERSION);
    if (!gContext->Init(false))
    {
        VERBOSE(VB_IMPORTANT, "Failed to init MythContext, exiting.");
        delete gContext;
        return GENERIC_EXIT_NO_MYTHCONTEXT;
    }

    bool refreshall     = cmdline.toBool("refresh-all");
    bool usedchanid     = cmdline.toBool("chanid");
    bool usedstarttime  = cmdline.toBool("starttime");

    uint chanid         = cmdline.toUInt("chanid");
    QString startstring = cmdline.toString("starttime");
    QDateTime starttime = myth_dt_from_string(startstring);

    if (refreshall && (usedchanid || usedstarttime))
    {
        VERBOSE(VB_IMPORTANT, "--refresh-all must not be accompanied by "
                              "--chanid or --starttime");
        return GENERIC_EXIT_INVALID_CMDLINE;
    }

    if (!refreshall && !(usedchanid && usedstarttime))
    {
        VERBOSE(VB_IMPORTANT, "--chanid and --starttime must be used together.");
        return GENERIC_EXIT_INVALID_CMDLINE;
    }

    if (!refreshall && !usedchanid && !usedstarttime)
    {
        refreshall = true;
    }

    myth_nice(19);

    MythTranslation::load("mythfrontend");

    LookerUpper *lookup = new LookerUpper();

    if (refreshall)
        lookup->HandleAllRecordings();
    else
        lookup->HandleSingleRecording(chanid, starttime);

    while (lookup->StillWorking())
    {
        sleep(1);
        qApp->processEvents();
    }

    delete lookup;
    delete gContext;

    VERBOSE(VB_IMPORTANT, "MythMetadataLookup run complete.");

    return GENERIC_EXIT_OK;
}
