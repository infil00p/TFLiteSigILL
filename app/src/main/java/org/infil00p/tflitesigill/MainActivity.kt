package org.infil00p.tflitesigill

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import kotlinx.android.synthetic.main.activity_main.*
import java.io.File
import java.io.FileOutputStream

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        var model = "mobilenet_v1_1_ondevice.tflite"
        var dataDirectory = this.filesDir
        var inputFile = this.assets.open(model)
        var outFile = File(dataDirectory, model)
        var outStream = FileOutputStream(outFile)
        inputFile.use { input ->
            outStream.use { fileOut ->
                input.copyTo(fileOut)
            }
        }

        // Example of a call to a native method
        sample_text.text = stringFromJNI()
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String

    companion object {

        // Used to load the 'native-lib' library on application startup.
        init {
            System.loadLibrary("native-lib")
        }
    }
}
