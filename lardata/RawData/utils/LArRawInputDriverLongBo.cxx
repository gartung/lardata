////////////////////////////////////////////////////////////////////////
/// \file  LArRawInputDriverLongBo.cxx
/// \brief Source to convert raw binary files to root files
///
/// \version $Id: LArRawInputDriverLongBO.h,
/// \author  brebel@fnal.gov, soderber@fnal.gov
////////////////////////////////////////////////////////////////////////

#include "lardata/RawData/utils/LArRawInputDriverLongBo.h"

#include "lardataobj/RawData/RawDigit.h"
#include "lardataobj/RawData/ExternalTrigger.h"
#include "lardataobj/RawData/DAQHeader.h"
#include "larcore/Geometry/Geometry.h"
#include "larcoreobj/SummaryData/RunData.h"

#include "art/Framework/IO/Sources/put_product_in_principal.h"
#include "canvas/Utilities/Exception.h"

extern "C" {
#include <sys/types.h>
#include <dirent.h>
}

// ======================================================================
// LongBo DAQ480 interface, adapted from code by Rebel/Soderberg:
//  modified M. Stancari Jan 4, 2013
namespace {

  //Define Structures corresponding to Binary data file.

  // ======================================================================
  struct header
  {
    int             fixed;     //Fixed 32-bit word with value:  0x0000D480
    unsigned short  format;    //File Format Version.  16-bit word.  Currently = 0x0001
    unsigned short  software;  //DAQ480 Software Version.  16-bit word.  Currently 0x0600 (v6.0)
    unsigned short  run;       //16-bit word.
    unsigned short  event;     //16-bit word.
    int             time;      //Event timestamp.  Coordinated Universal Time. 32-bit word.
    short           spare;     //Spare 16-bit word.  Currently 0x0000
    unsigned short  nchan;     //Total # of channels in readout. 16-bit word.
  };

  // ======================================================================
  struct channel
  {
    unsigned short  ch;        //Channel #.  16-bit word.
    unsigned short  samples;   //# samples for this channel.   16-bit word.
  };

  // ======================================================================
  struct footer
  {
    int             spare;     //Spare 32-bit word.  Currently 0x00000000
    int             checksum;  //Reserved for checksum.  32-bit word.  Currently 0x00000000
  };

  // ======================================================================
  int run( std::string s1 )
  {
    size_t p1 = s1.find("R");
    size_t p2 = s1.find("_E");

    int run = atoi((s1.substr(p1+1,p2-p1-1)).c_str());
    return run;
  }


  // ======================================================================
  int event( std::string s1 )
  {
    size_t p1 = s1.find("E");
    size_t p2 = s1.find("_T");

    int event = atoi((s1.substr(p1+1,p2-p1-1)).c_str());
    return event;
  }


  // ======================================================================
  bool compare( std::string s1, std::string s2 )
  {
    int r1 = run(s1);
    int r2 = run(s2);
    int e1 = event(s1);
    int e2 = event(s2);

    return r1 == r2 ?  e1 < e2
      :  r1 < r2;
  }


  // ======================================================================
  std::vector<std::string> getsortedfiles( std::string dir )
  {
    if( dir == "" )
      throw art::Exception( art::errors::Configuration )
        << "Vacuous directory name" << std::endl;

    std::vector<std::string> files;

    DIR * dp = NULL;
    if( (dp = opendir(dir.c_str())) == NULL ) {
      throw art::Exception( art::errors::FileOpenError )
        << "Error opening directory " << dir << std::endl;
    }

    dirent * dirp = NULL;
    while( (dirp = readdir(dp)) != NULL ) {
      std::string filename( dirp->d_name );
      if( filename.find("bin") != std::string::npos ) {
        files.push_back(filename);
      }
    }
    closedir(dp);

    sort( files.begin(), files.end(), compare );

    return files;
  }  // getsortedfiles()

  struct EventFileSentry {
    // Use RAII (Resource Acquisition Is Initialization)
    explicit EventFileSentry(std::string const &filepath)
      : infile(filepath.c_str(), std::ios_base::in | std::ios_base::binary)
    { }
    ~EventFileSentry() { infile.close(); }

    std::ifstream infile;
  };

  // ======================================================================
  void process_LAr_file(std::string dir,
                        std::string  const &  filename,
                        std::vector<raw::RawDigit>& digitList,
                        raw::DAQHeader& daqHeader,
                        std::vector<raw::ExternalTrigger>& extTrig)
  {
    // Prepare the input file. The sentry is responsible for making the
    // file stream object, and will *automatically* close it when it
    // goes out of scope *for any reason*, including normal function
    // exit or exception throw.
    EventFileSentry efs(dir+"/"+filename);
    std::ifstream &infile = efs.infile;
    
    if( !infile.is_open() ) {
      throw art::Exception( art::errors::FileReadError )
        << "failed to open input file " << filename << std::endl;
    }
    
    ///\todo Total number of channels=144 in Long Bo is hardcoded in LArRawInputDriver_LongBo.cxx
    unsigned int wiresPerPlane = 48;
    unsigned int planes = 3;
    int nwires = wiresPerPlane*planes;
    
    header h1;
    channel c1;
    //    footer f1;
    
    //read in header section of file
    infile.read((char *) &h1, sizeof h1);
    
    time_t mytime = h1.time;
    mytime = mytime << 32;//Nov. 2, 2010 - "time_t" is a 64-bit word on many 64-bit machines
    //so we had to change types in header struct to read in the correct
    //number of bits.  Once we have the 32-bit timestamp from the binary
    //data, shift it up to the upper half of the 64-bit timestamp.  - Mitch
    
    // std::cout << "Fixed Value (0x0000D480): " << std::hex << h1.fixed << std::endl;
    // std::cout << "Output Format: " << std::hex << h1.format << std::endl;
    // std::cout << "Software Version: " << std::hex << h1.software << std::dec << std::endl;
    // std::cout << "Run " << std::setw(6) << std::left << h1.run
    //     << "Event " << std::setw(8) << std::left << h1.event
    //           << "h1.time " << std::setw(8) << std::left << h1.time;
    // std::cout << " #Channels = " << h1.nchan << std::endl;
    
    daqHeader.SetStatus(1);
    daqHeader.SetFixedWord(h1.fixed);
    daqHeader.SetFileFormat(h1.format);
    daqHeader.SetSoftwareVersion(h1.software);
    daqHeader.SetRun(h1.run);
    daqHeader.SetEvent(h1.event);
    daqHeader.SetTimeStamp(mytime);
    daqHeader.SetSpareWord(h1.spare);
    daqHeader.SetNChannels(h1.nchan);
    
    //one digit for every wire on each plane
    digitList.clear();
    digitList.resize(wiresPerPlane*planes);
    
    //16 external trigger inputs
    extTrig.clear();
    extTrig.resize(16);
    
    for( int i = 0; i != nwires; ++i ) {
      infile.read((char *) &c1, sizeof c1);
      //Create vector for ADC data, with correct number of samples for this event
      std::vector<short> adclist(c1.samples);
      infile.read((char*)&adclist[0],sizeof(short)*c1.samples);
      //      std::cout << "Channel = " << c1.ch ;
      // std::cout << " #Samples = " << c1.samples ;
      // std::cout << " ADC[0] = " << adclist[0] << " ADC[2047] = " << adclist[2047] << std::endl;
      
      // set signal to be 400 if it is 0 (bad pedestal)
      for (int ijk=0;ijk<c1.samples;++ijk) {
	if (std::abs(adclist[ijk])<1e-5)
	  adclist[ijk]+=400;
      }

      // invert the signals from the BNL ASIC
      if (i>63 && i<80) {
	for (int ijk=0;ijk<c1.samples;++ijk) {
	  int mysig=adclist[ijk]-400;
	  adclist[ijk]=400-mysig;
	}
      }
      
      if (i<96){
	//      digitList[i] = raw::RawDigit((c1.ch-1), c1.samples, adclist);//subtract one from ch. number...
	digitList[i] = raw::RawDigit(i, c1.samples, adclist);//subtract one from ch. number...
	//hence offline channels will always be one lower
	//than the DAQ480 definition. - mitch 7/8/2009
	digitList[i].SetPedestal(400.); //carl b assures me this will never change. bjr 4/15/2009
      }
      else{//flip collection wires to be consistent with offline geometry. TYang 12/23/2013
	digitList[239-i] = raw::RawDigit(239-i, c1.samples, adclist);
	digitList[239-i].SetPedestal(400.); //carl b assures me this will never change. bjr 4/15/2009
      }
    }

    //
    //  MStancari, TYang - Apr 4 2013
    //
    //  Add trigger information to the record
    
    unsigned int ichan;
    for( int i = 0; i < 16; ++i ) {
      unsigned int utrigtime = 0;      
      infile.read((char *) &c1, sizeof c1);
      //Create vector for ADC data, with correct number of samples for this event
      std::vector<short> adclist(c1.samples);
      infile.read((char*)&adclist[0],sizeof(short)*c1.samples);
      
      int j=0;
      while (j<c1.samples){      
	float test = 400.0-adclist[j];
	if (test>10 && j>0) {
	  utrigtime=j;
	  break;
	}
	j++;
      }
      ichan=i+144;
      extTrig[i] = raw::ExternalTrigger(ichan,utrigtime);
    }
    


    // infile will be closed automatically as EventFileSentry goes out of scope.
  }  // process_LAr_file

} // namespace

namespace lris {
  // ======================================================================
  // class c'tor/d'tor:
  LArRawInputDriverLongBo::LArRawInputDriverLongBo(fhicl::ParameterSet const &, // Not used
                                       art::ProductRegistryHelper &helper,
                                       art::SourceHelper const &pm)
    :
    principalMaker_(pm)
    , currentDir_        ()
    , inputfiles_        ( )
    , nextfile_          ( inputfiles_.begin() )
    , filesdone_         ( inputfiles_.end() )
    , currentSubRunID_   ( )
  {
    helper.reconstitutes<raw::DAQHeader,              art::InEvent>("daq");
    helper.reconstitutes<std::vector<raw::RawDigit>,  art::InEvent>("daq");
    helper.reconstitutes<std::vector<raw::ExternalTrigger>,art::InEvent>("daq");
    helper.reconstitutes<sumdata::RunData,            art::InRun>  ("daq");
  }

  void LArRawInputDriverLongBo::closeCurrentFile()
  {
    // Nothing to do (See EventFileSentry).
  }

  void LArRawInputDriverLongBo::readFile(std::string const &name,
                                   art::FileBlock* &fb)
  {
    // Get the list of event files for this directory.
    currentDir_ = name;
    inputfiles_ = getsortedfiles(currentDir_);
    nextfile_ = inputfiles_.begin();
    filesdone_ = inputfiles_.end();
    currentSubRunID_ = art::SubRunID();

    // Fill and return a new Fileblock.
    fb = new art::FileBlock(art::FileFormatVersion(1, "LArRawInput 2011a"),
                            currentDir_);
  }

  bool LArRawInputDriverLongBo::readNext(art::RunPrincipal* const & /* inR */,
                                   art::SubRunPrincipal* const & /* inSR */,
                                   art::RunPrincipal* &outR,
                                   art::SubRunPrincipal* &outSR,
                                   art::EventPrincipal* &outE)
  {
    if (inputfiles_.empty() || nextfile_ == filesdone_ ) return false;

    // Create empty result, then fill it from current filename:
    std::unique_ptr<std::vector<raw::RawDigit> >  rdcollb ( new std::vector<raw::RawDigit>  );
    std::unique_ptr<std::vector<raw::ExternalTrigger> >  etcollb ( new std::vector<raw::ExternalTrigger>  );

    raw::DAQHeader daqHeader;
    bool firstEventInRun = (nextfile_ == inputfiles_.begin());

    process_LAr_file( currentDir_, *nextfile_++, *rdcollb, daqHeader, *etcollb);
    std::unique_ptr<raw::DAQHeader>              daqcollb( new raw::DAQHeader(daqHeader) );

    art::RunNumber_t rn = daqHeader.GetRun();
    art::Timestamp tstamp = daqHeader.GetTimeStamp();

    if (firstEventInRun)
      {
	std::unique_ptr<sumdata::RunData> rundata(new sumdata::RunData("bo") );
        currentSubRunID_ = art::SubRunID(rn, 1);
        outR = principalMaker_.makeRunPrincipal(rn, tstamp);
        outSR = principalMaker_.makeSubRunPrincipal(rn,
                                                    currentSubRunID_.subRun(),
                                                    tstamp);
        art::put_product_in_principal(std::move(rundata), *outR, "daq");
      } else if (rn != currentSubRunID_.run())
      {
        throw cet::exception("InconsistentEventStream")
          << "Encountered run #" << rn
          << " while processing events from run #" << currentSubRunID_.run()
          << "\n";
      }

    outE = principalMaker_.makeEventPrincipal(currentSubRunID_.run(),
                                              currentSubRunID_.subRun(),
                                              daqHeader.GetEvent(),
                                              tstamp);

    // Put products in the event.
    art::put_product_in_principal(std::move(etcollb),
                                  *outE,
                                  "daq"); // Module label
    art::put_product_in_principal(std::move(rdcollb),
                                  *outE,
                                  "daq"); // Module label
    art::put_product_in_principal(std::move(daqcollb),
                                  *outE,
                                  "daq"); // Module label

    return true;
  }

}
