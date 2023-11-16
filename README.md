# ExpressoMachine

To run this project you need JUCE framework. In ProJucer create new project and add DSP module. Then add all files to you project. 
Program has been built with Visual Studio 2022, and JUCE framework with C++.

Some basic audio sample

Clean tone:
https://github.com/bartek98205/ExpressoMachine/assets/100700947/af1a4948-84fe-4b1a-8973-386879cf189b

with running program:
https://github.com/bartek98205/ExpressoMachine/assets/100700947/e1fb17c5-b2a2-4ed8-984e-f4b53c17449a

with running program:
https://github.com/bartek98205/ExpressoMachine/assets/100700947/325dca0a-4ac1-480d-8347-9f517328f2a0

This tool evaluates the input signal, applying tailored harmonics and filtering to enhance the audio.
The extent of sound enrichment is dynamically influenced by the user's performance. 
Real-time audio signal adjustments provide musicians with a fresh avenue for self-expression during their performances.

This tool allows you to add specific harmonics to guitar signal. 

Firstly the signal is being analyzed, after that program adds specific tones to the signal. 
The program responds to the guitar playing's force along with increasing playing dynamics, and it begins to add more and more tones to the signal.
So the louder you play the more tones are beign added to the signal. 
Program works correclty.  

![obraz](https://github.com/bartek98205/ExpressoMachine/assets/100700947/d8ae8ab9-a895-4b49-848b-6966897d4f6d)

As you can see on the plot, the program has added specific tones to the signal, making it sound more interesting.
