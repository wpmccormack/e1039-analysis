R__LOAD_LIBRARY(libana_embedding)
using namespace std;

void AnaCleanAndMessy()
{
  AnaCleanAndMessyData* ana = new AnaCleanAndMessyData();
  ana->Init("not_embedded/ana_tree.root", "embedded/ana_tree.root");
  ana->Analyze();
  ana->End();
  exit(0);
}
