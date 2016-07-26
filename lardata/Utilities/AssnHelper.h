/*
 *
 *  @brief  Helper function to access associations in order
 *
 */

#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "cetlib/exception.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

// this namespace will be moved to a different file
namespace lar {
   struct Range {
      size_t begin_ {};
      size_t end_ {};
      Range() = default;
      Range(size_t b, size_t e) : begin_(b), end_(e) { }
   };
   
   template <class L, class R>
   class AssnHelper {
   public:
      //Creates a AssnHelper that allows ordered access to association collection
      AssnHelper(art::Event const& evt,
                 std::string const & inputLabel = std::string());
      
      void calcRange();
      Range getMany(size_t index);
      void associatedObjects(Range r);
      
   private:
      const art::ValidHandle<art::Assns<L, R>> assnsHandle;
      std::vector<Range> assnsRanges {};
   };
   
   // COnstructor
   template <class L, class R>
   AssnHelper<L,R>::AssnHelper(art::Event const& evt, std::string const & inputLabel)
   : assnsHandle(evt.getValidHandle<art::Assns<L, R>> (inputLabel)),
   assnsRanges()
   {  calcRange(); }
   
   // Calculate ranges for R for each L
   template <class L, class R>
   void AssnHelper<L,R>::calcRange() {
      auto assns(*assnsHandle);
      size_t iLeft = 0;
      size_t iRight = 0;
      auto it = assns.begin();
      Range rg;
    //  mf::LogVerbatim("NueAna") << "Track: " << iLeft << ", Associated hit starts at: " << iRight;
      for (size_t t = 0; t < assns.size(); ++t )
      {
         auto const &p = *it;
         if(iLeft != p.first.key()){
      //      mf::LogVerbatim("NueAna") << "Track: " << p.first.key() << ", Associated hit starts at: " << t;
            rg.begin_ = iRight;
            rg.end_ = t;
            assnsRanges.emplace_back(rg);
            iRight = t;
         }
         iLeft = p.first.key();
         ++it;
      }
      //For the last range
      rg.begin_ = rg.end_;
      rg.end_ = assns.size();
      assnsRanges.emplace_back(rg);
      
      // debug print
      for (size_t a=0; a< assnsRanges.size(); ++a) {
         mf::LogVerbatim("NueAna") << a << ": " << assnsRanges[a].begin_ << ", " << assnsRanges[a].end_;
      }
   }
   
   // Given an index of L, return range of R
   template <class L, class R>
   Range AssnHelper<L,R>::getMany(size_t index) {
      return assnsRanges[index];
   }
   
   //given the range of R for L, print associated Rs
   template <class L, class R>
   void AssnHelper<L, R>::associatedObjects(Range r) {
      auto assns(*assnsHandle);
      mf::LogVerbatim("NueAna") << "Track #" << assns[r.begin_].first.key();
      for (size_t i = r.begin_; i!= r.end_; ++i) {
         mf::LogVerbatim("NueAna") << assns[i].second.key();
      }
      //mf::LogVerbatim("NueAna") << assns[r.begin_].first.key();
      //return &assns;
   }
}
// end of namespace
