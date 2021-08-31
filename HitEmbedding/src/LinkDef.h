#ifdef __CINT__
#pragma link off all class;
#pragma link off all function;
#pragma link off all global;

// TreeData.h
#pragma link C++ class EventData+;
#pragma link C++ class TrackData+;
#pragma link C++ class DimuonData+;
#pragma link C++ class TrackList+;
#pragma link C++ class DimuonList+;

// EmbeddingData.h
#pragma link C++ class EmbEventData+;
#pragma link C++ class EmbHitData+;
#pragma link C++ class EmbSimEventData+;
#pragma link C++ class EmbSimTrackData+;
#pragma link C++ class EmbSimDimuonData+;
#pragma link C++ class EmbHitList+;
#pragma link C++ class EmbSimTrackList+;
#pragma link C++ class EmbSimDimuonList+;

#pragma link C++ class EventFilter-!;
#pragma link C++ class GenEmbeddingData-!;
#pragma link C++ class DoEmbedding-!;
#pragma link C++ class AnaEmbeddedData-!;

#endif
