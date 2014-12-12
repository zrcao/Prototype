#include <uhd/utils/thread_priority.hpp>
#include <uhd/utils/safe_main.hpp>
#include <uhd/utils/static.hpp>
#include <uhd/usrp/multi_usrp.hpp>
//#include <uhd/types/ranges.hpp>
#include <uhd/exception.hpp>
#include <boost/program_options.hpp>
#include <boost/math/special_functions/round.hpp>
#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <csignal>

namespace po = boost::program_options;

/*****************************************
*  Main functions
*****************************************/

int UHD_SAFE_MAIN(int argc, char *argv[]){
    uhd::set_thread_priority_safe();

    //variables
    std::string args, channel_list, ref;


    //
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "help message")
        ("args", po::value<std::string>(&args)->default_value(""), "single uhd device address args")
        ("ref", po::value<std::string>(&ref)->default_value("internal"), "clock reference (internal, external, mimo)")
    ;
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

   // print the help message
   if (vm.count("help"))
      std::cout << boost::format("UHD Study %s") % desc << std::endl;

   // create a usrp device
   std::cout << std::endl;
   std::cout << boost::format("Creating the usrp device with %s ...") % args << std::endl;
   uhd::usrp::multi_usrp::sptr usrp = uhd::usrp::multi_usrp::make(args);

   // Since the RF daughter cards we would use only have one channel in each direction
   size_t ch = 0;

   // Lock mboard clock
   usrp->set_clock_source(ref);
  
   std::cout << "--------------------------------------------------" << std::endl;
   std::cout << boost::format("The number of TX channels %d.") % usrp->get_tx_num_channels() << std::endl;
   std::cout << boost::format("The number of RX channels %d.") % usrp->get_rx_num_channels() << std::endl;
   std::cout << "--------------------------------------------------" << std::endl;
   std::cout << boost::format("The TX channel analog bandwidth is %f MHz.") % (usrp->get_tx_bandwidth(ch)/1e6) << std::endl;
   std::cout << boost::format("The Tx transmitter frequency is at %f MHz.") % (usrp->get_tx_freq(ch)/1e6) << std::endl;
   uhd::freq_range_t freq_range=usrp->get_tx_freq_range(ch); 
   std::cout << boost::format("The Tx transmitter frequency range is %.3f to %.3f  MHz.") % (freq_range.start()/1e6) % (freq_range.stop()/1e6) << std::endl;
   std::cout << "--------------------------------------------------" << std::endl;
   std::cout << boost::format("The TX gain is %f dB.") % usrp->get_tx_gain(ch) << std::endl;

   return EXIT_SUCCESS;
}
