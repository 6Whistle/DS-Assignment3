#ifndef RESULT_H
#define RESULT_H

enum Result
{
    Success = 0,
    InvalidOptionNumber = 1,
    FaildtoUpdatePath = 5,
    CommandFileNotExist = 100,
    LoadFileNotExist,

    VertexKeyNotExist = 201,
    InvalidVertexKey = 200,
    GraphNotExist = 202,
    InvalidAlgorithm,
    NegativeCycleDetected,

    NonDefinedCommand = 300,
};

#endif