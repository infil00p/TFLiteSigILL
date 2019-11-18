#include <jni.h>
#include <string>
#include <iostream>
#include <fstream>

#include "tensorflow/lite/kernels/register.h"
#include "tensorflow/lite/model.h"

std::string MODEL_PATH = "/data/data/org.infil00p.tflitesigill/files/mobilenet_v1_1_ondevice.tflite";

extern "C" JNIEXPORT jstring JNICALL
Java_org_infil00p_tflitesigill_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {

    std::string result = "Nothing ran yet";

    auto model = ::tflite::FlatBufferModel::BuildFromFile(MODEL_PATH.c_str());
    ::tflite::ops::builtin::BuiltinOpResolver resolver;
    ::tflite::InterpreterBuilder builder(*model, resolver);

    std::unique_ptr<::tflite::Interpreter> interpreter;
    if(builder(&interpreter) == kTfLiteOk) {
        result = "Things ran well for building the interpreter";
    }

    if(interpreter->AllocateTensors() == kTfLiteOk) {
        result = "Things ran well for allocation";
    }

    auto inputIndex = interpreter->inputs()[0];
    auto inputTensor = interpreter->tensor(inputIndex);
    std::vector<float> input(inputTensor->bytes);
    for(int i = 0; i < inputTensor->bytes; ++i) {
        input[i] = 0;
    }
    // Set everything to zero
    memcpy(inputTensor->data.raw, input.data(), inputTensor->bytes);

    // This should cause a SIGILL on v2.0.0, but otherwise work.
    if(interpreter->Invoke() == kTfLiteOk) {
        result = "Invocation worked without a crash";
    }

    return env->NewStringUTF(result.c_str());

}
