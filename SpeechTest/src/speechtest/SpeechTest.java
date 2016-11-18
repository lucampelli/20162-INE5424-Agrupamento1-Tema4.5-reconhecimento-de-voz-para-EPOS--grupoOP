/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package speechtest;

import java.io.File;
import java.io.FileInputStream;
import java.io.InputStream;

import edu.cmu.sphinx.api.Configuration;
import edu.cmu.sphinx.api.SpeechResult;
import edu.cmu.sphinx.api.StreamSpeechRecognizer;

import java.io.PrintWriter;
import static java.lang.Thread.sleep;

public class SpeechTest {

    public static void main(String[] args) throws Exception {

        Configuration configuration = new Configuration();

        configuration
                .setAcousticModelPath("resource:/edu/cmu/sphinx/models/en-us/en-us");
        configuration
                .setDictionaryPath("resource:/edu/cmu/sphinx/models/en-us/cmudict-en-us.dict");
        configuration
                .setLanguageModelPath("resource:/edu/cmu/sphinx/models/en-us/en-us.lm.bin");

        StreamSpeechRecognizer recognizer = new StreamSpeechRecognizer(
                configuration);
        File f = new File("/home/"+System.getProperty("user.name")+"/Test.wav");
        while(!f.isFile()){
            sleep(100);
            f = new File("/home/"+System.getProperty("user.name")+"/Test.wav");
        }
        InputStream stream = new FileInputStream(f);

        recognizer.startRecognition(stream);
        SpeechResult result;
        while ((result = recognizer.getResult()) != null) {
            String answer = result.getHypothesis();
            System.out.format("Hypothesis: %s\n", answer);
            try {
                PrintWriter writer = new PrintWriter("/home/"+System.getProperty("user.name")+"/Comand.txt", "UTF-8");
                writer.write(answer);
                writer.close();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        recognizer.stopRecognition();
    }

}
