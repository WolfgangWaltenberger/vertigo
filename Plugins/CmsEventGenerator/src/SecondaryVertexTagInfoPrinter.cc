#include "SecondaryVertexTagInfoPrinter.h"
#include <string>
#include <cassert>
#include "TBranch.h"
#include "TLeaf.h"
#include <rave/impl/FWCore/MessageLogger/interface/MessageLogger.h>
#include <rave/impl/DataFormats/BTauReco/interface/SecondaryVertexTagInfo.h>

using namespace std;

namespace {
  void printSVData( const string & identifier, TTree * tree, int event )
  {
    /**
     *  Should print the Secondary Vertex Data, but it doesnt work.
     *  Maybe it has to do with the fact that its an STL vector.
     */
    std::vector < reco::SecondaryVertexTagInfo::VertexData > vecdata;
    TBranch * b_svdata= tree->GetBranch ( ( identifier + "m_svData" ).c_str() );
    if (!b_svdata) return;
    // reco::SecondaryVertexTagInfo::VertexData * data; 
    b_svdata->SetAddress ( &vecdata );
    b_svdata->GetEntry(event);

    if ( vecdata.size() )
      cout << "[SecondaryVertexTagInfoPrinter] data vtx=" << vecdata[0].vertex.x() << ", " << vecdata[0].vertex.y()
           << ", " << vecdata[0].vertex.z() << endl;
  }

  void printTagValue ( TTree * tree, int event )
  {
    /**
     *  print the discriminator values.
     */
    vector < float > values;
    TBranch * b = tree->GetBranch("recoJetedmRefToBaseProdTofloatsAssociationVector_simpleSecondaryVertexBJetTags__ALL.obj.data_");
    if (!b) return;
    b->SetAddress(&values);
    b->GetEntry(event);
    // cout << "[SecondaryVertexTagInfoPrinter] jettags=" << values.size() << endl;
    for ( unsigned i=0; i< values.size() ; i++ )
    {
      if (values[i]>-.5) cout << "[SecondaryVertexTagInfoPrinter] d(" << i << ")=" << values[i] << endl;
    }
  }
}

void SecondaryVertexTagInfoPrinter::print( TTree * tree, int event )
{
  cout << "[SecondaryVertexTagInfoPrinter] printing event " << event << endl;
  string identifier="recoSecondaryVertexTagInfos_secondaryVertexTagInfos__ALL.obj.";
  TLeaf * l_vcs= tree->GetLeaf ( ( identifier + "m_vertexCandidates" ).c_str() );
  if ( (!l_vcs) ) return;
  // int events = l_vcs->GetBranch()->GetEntries();
  // cout << "[SecondaryVertexTagInfoPrinter] I see " << events << " events." << endl;
  l_vcs->GetBranch()->GetEntry(event);
  // b_svdata->GetEntry(event);
  int entries = l_vcs->GetLen();

  cout << "[SecondaryVertexTagInfoPrinter] I see " << entries << " TagInfos for event " 
       << event << "." << endl;
  for ( int i=0; i< entries ; i++ )
  {
    int ncandidates = l_vcs->GetValue(i);

    if ( ncandidates>0 )
    {
      cout << "[SecondaryVertexTagInfoPrinter] Tag " << i << " has " 
           << ncandidates << " vertex candidates." << endl;
    }

  }

  // printSVData ( identifier, tree, event );
  printTagValue ( tree, event );
}
