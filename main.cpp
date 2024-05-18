#include "maximilian.h"

//MAXIMICHAEL - Billie Jean

//Within the code I have re-created billie jean by michael jackson using only maximillian to.
//I have used samples which I have applied effects to via reason 12.
//All synthesis and sequencing has been done using maximillian.



/////////////////////////////////////////////////////////////////////////
//Track Settings

//startTrack in different place(0 - start song from the beginning, any other number to begin at a certain bar in the piece
int currentBar = 0;

//ends the track
bool stopPlaying;
int tempo = 116; //this will not affect the samples as they have been pre-processed

//16th notes clocks
maxiClock sixteenClock;
//16 notes number
int stepCount;


//8th notes clock
maxiClock bassClock;

//Notes to frequency

//Used for bass
double FSharp = 185;
double CSharp = 139;
double E = 164;
double B = 123;
double A = 110;

//used for synth and guitar
double FSharp2 = 740;
double AFlat = 830;
double A2 = 880;
double CSharp2 = 554;
double B2 = 494;
double A3 = 440;




/////////////////////////////////////////////////////////////////////////
//Bass part

//selects which bass part is being played
int bassPart = 0;
//current bass step
int bassStep;
//notes
float bassFreq;
//Bass notes
float bassLine[3][8] = {{CSharp,FSharp,CSharp,E,FSharp,E,CSharp,B},
    {FSharp,B,FSharp,A,B,A,FSharp,E},
    {CSharp,B,B,A,A,CSharp,CSharp,CSharp}};

//parameters
float bassPulseWidth = 0.6;

maxiOsc VCO1,VCO2,LFO1,LFO2;
maxiFilter VCF;
maxiEnv ADSR;
maxiFilter lowpassFilter;

double VCO1out,VCO2out,LFO1out,LFO2out,VCFout,ADSRout;

/////////////////////////////////////////////////////////////////////////
//Synth part
maxiOsc VCO3,VCO4,LFO3,LFO4;
maxiFilter VCF1;
maxiEnv ADSR1;

double VCO3out, VCO4out, LFO3out,LFO4out,VCF1out,ADSR1out;

//synth parameters
float synthFreq;
int synthPart;

//guitarSwitch
bool guitarPlay;


/////////////////////////////////////////////////////////////////////////
//sampling
//triggers samples
bool billieJeanPlay;
bool theOnePlay;
bool moreLikePlay;
bool tellMePlay;

//stored samples
maxiSample billieJean;
maxiSample theOne;
maxiSample moreLike;
maxiSample tellMe;

//signals used to output samples
double billieJeanSampleSignal = 0;
double theOneSampleSignal = 0;
double moreLikeSampleSignal = 0;
double tellMeSampleSignal = 0;


/////////////////////////////////////////////////////////////////////////
//Drums
//Drum parameters
float kickSpeed = 1;
float snareSpeed = 2.5;
//Drum samples
maxiSample drum;
maxiSample snare;

//starts drum samples
bool startKick;
bool startSnare;


//End of global variables







/////////////////////////////////////////////////////////////////////////
//function which plays drums
double playDrum(int speed)
{
    double output;
    
    if(speed == 1)
    {
       output = drum.playOnce();
    }
    
    if(speed > 1)
    {
        output = snare.playOnce(5);
    }
    return output;
}



/////////////////////////////////////////////////////////////////////////
///This function plays the first drum pattern by playing a sample at certain steps
void drumPatterOne()
{
    if(sixteenClock.tick)
    {
        //kick
        if(stepCount == 1 || stepCount == 4 || stepCount == 11)
        {
            drum.setPosition(0);
            startKick = true; //this will play the kick drum
        }
        //snare
        if(stepCount == 5 || stepCount == 13)
        {
            snare.setPosition(0);
            startSnare = true; // this will play the snare drum
        }
        
        if(stepCount == 7 || stepCount == 15)
        {
            startSnare = false;
            snare.setPosition(0); //stops snare
        }
    }
}

/////////////////////////////////////////////////////////////////////////
///This function plays the second drum pattern by playing a sample at certain steps
void drumPatterTwo()
{
    if(sixteenClock.tick)
    {
        //kick
        if(stepCount == 1 || stepCount == 9)
        {
            drum.setPosition(0);
            startKick = true;
        }
        //snare
        if(stepCount == 5 || stepCount == 13)
        {
            snare.setPosition(0);
            startSnare = true;
        }
        
        if(stepCount == 7 || stepCount == 15)
        {
            startSnare = false;
            snare.setPosition(0); //stops snare
        }
    }
}


/////////////////////////////////////////////////////////////////////////
///This function will play a sample and assign this to a double which is later added to the output
void samplePlayer()
{
    if(billieJeanPlay)
    {
        billieJeanSampleSignal = billieJean.play();
    }
    if(moreLikePlay)
    {
        moreLikeSampleSignal = moreLike.play();
    }
    if(tellMePlay)
    {
        tellMeSampleSignal = tellMe.play();
    }
    if(theOnePlay)
    {
        theOneSampleSignal = theOne.play();
    }
}



/////////////////////////////////////////////////////////////////////////
///This function sequences samples and parameters of the entire track by using the currentbar instead of a step in a sequence. This is where the track has been organised.
void masterSequencer()
{
    sixteenClock.ticker(); //This clock is the 16th note clock used for drums and synths
    
    if(sixteenClock.tick)
    {
        if(stepCount == 1)
        {
            currentBar++; // increments the current bar if 16 step sequence is 1
            std::cout<<currentBar<<std::endl;
        }
        
        //drum sequencing
        if(currentBar > 4 && currentBar < 18)
        {
            //first section of dubstyle michael jackson beat
            drumPatterOne();
            ADSR.setAttack(10000);
            ADSR1.setAttack(10000);
            
        }
        if(currentBar > 18) // bridge section
        {
            drumPatterTwo();
            ADSR.setAttack(1000);
            ADSR1.setAttack(1000);
            bassPart = 1;
            bassPulseWidth = 3;
        }
        if(currentBar > 22 && currentBar < 27) // back to verse section
        {
            drumPatterOne();
            ADSR.setAttack(10000);
            ADSR1.setAttack(10);
            bassPart = 0;
            bassPulseWidth = 0.8;
            guitarPlay = true; //guitar inspired synth riff
        }
        
        if(currentBar > 26 && currentBar < 31) //back to bridge with shorter attack times
        {
            drumPatterTwo();
            bassPart = 2;
            ADSR.setAttack(100);
            ADSR1.setAttack(100);
            guitarPlay = false;
        }
        if(currentBar >= 31)
        {
            guitarPlay = true;
            ADSR1.setAttack(10000);
            ADSR.setAttack(10000);
            bassPart = 0;
            bassPulseWidth = 0.8;
        }
        
        if(currentBar == 24)
        {
            bassPulseWidth = 3;
        }
        if(currentBar == 25)
        {
            bassPulseWidth = 0.6;
        }
       
        if(currentBar == 33)
        {
            stopPlaying = true; //stops the track at bar 33
        }
        
    
    
        
        //SAMPLING - Michael Jackson voice samples
  
        //She was more like
        if(currentBar == 5 || currentBar == 7 || currentBar == 9 || currentBar == 11)
        {
            moreLikePlay = true; //when this boolean is true it will play the sample
        }else
        {
            moreLikePlay = false;
            moreLike.setPosition(0);
            moreLikeSampleSignal = 0;
        }
        
        
        //I am the One
        if(currentBar == 12 || currentBar == 16 || currentBar == 28)
        {
            theOnePlay = true;
        }else
        {
           theOnePlay = false;
            theOne.setPosition(0);
            theOneSampleSignal = 0;
        }
        
        //Billie jean is not my lover
        if(currentBar == 13 || currentBar == 14 || currentBar == 17 || currentBar == 18 || currentBar == 31 || currentBar == 32)
        {
            billieJeanPlay = true;
        }
        else
        {
           billieJeanPlay = false;
            billieJean.setPosition(0);
            billieJeanSampleSignal = 0;
        }
        
        //people always tell me be careful what you do.....
        
        //This section uses greater than symbols as the sample is longer
        {
            if((currentBar > 18 && currentBar < 23) || (currentBar > 26 && currentBar < 31))
            {
                tellMePlay = true;
            }
                else
                {
                   tellMePlay = false;
                    tellMe.setPosition(0);
                    tellMeSampleSignal = 0;
                }
        }
    }

}

/////////////////////////////////////////////////////////////////////////
///Initialises some variables
void setup() {//some inits
    
    //Track is starting
    stopPlaying = false;
    
    //Loads samples - Change this path to the path of the samples
    
    drum.load("/Users/user/Desktop/Maximilian/kick2.wav");//load in your samples.
    snare.load("/Users/user/Desktop/Maximilian/snare.wav");//load in your samples.
    billieJean.load("/Users/user/Desktop/Maximilian/billieJean.wav");
    theOne.load("/Users/user/Desktop/Maximilian/iAmTheOne.wav");
    moreLike.load("/Users/user/Desktop/Maximilian/sheWasMoreLike.wav");
    tellMe.load("/Users/user/Desktop/Maximilian/peopleAlwaysTellMe.wav");
    
    //switches from synth riff to guitar riff
    guitarPlay = false;
    
    //Initalises step-sequencer
    stepCount = 0;
    
    
    //Bass ADSR
    ADSR.setAttack(10);
    ADSR.setDecay(1);
    ADSR.setSustain(1);
    ADSR.setRelease(250);
    
    //Synth and guitar ADSR
    ADSR1.setAttack(10);
    ADSR1.setDecay(1);
    ADSR1.setSustain(1);
    ADSR1.setRelease(700);

    
    //Initalises step sequencer clock
    sixteenClock.setTicksPerBeat(4);
    sixteenClock.setTempo(tempo);
    
    //Initalises bass clock and step
    bassClock.setTicksPerBeat(2);
    bassClock.setTempo(tempo);
    bassStep = 1;
}

/////////////////////////////////////////////////////////////////////////
///This function runs 44100 times a second therefore I have used booleans and other techniques to create other functions which trigger notes and samples
void play(double *output) {
    
    masterSequencer(); //Function to structure the track
    
    samplePlayer(); //plays the samples and sends to output

    bassClock.ticker(); //Continues bass clock
  
    
    //Looping step sequence
    //Synth track consists of 32 steos therefore I have created synth part to create a looping riff
    if(sixteenClock.tick && stepCount < 15)
        {
            stepCount++;
        }
        else if(sixteenClock.tick && stepCount == 15)
        {
            stepCount = 0; //starts a loop from 0 to 15
            synthPart++; //the synth loop is over the course of two bars so this has to change
        }
    
    if(synthPart == 2)
    {
        synthPart = 0; //synth looping
    }
    
    
    
    //Looping the bass step
    if(bassClock.tick && bassStep < 8)
        {
            bassStep++;
            //std::cout << bassStep << std::endl;
        }
        else if(sixteenClock.tick && bassStep == 8)
        {
            bassStep = 0;
        }
    
    
  
    
    
    //Bass synthesis
    if(bassClock.tick)
    {
        ADSR.trigger = 1;
    }
    
    bassFreq = bassLine[bassPart][bassStep - 1];
    ADSRout=ADSR.adsr(1.0,ADSR.trigger);
    LFO1out=LFO1.sinebuf(0.2);//this lfo is a sinewave at 0.2 hz
    
    VCO1out=VCO1.pulse((bassFreq / 2),bassPulseWidth);
    VCO2out=VCO2.pulse(bassFreq+LFO1out,0.2); //oscillators
    
    
    VCFout=VCF.lores((VCO1out+VCO2out)*0.5, ADSRout*600, 1);//Resonant filter controlled by the ADSR
    
    double bassSound=VCFout*ADSRout;// This is the double for the final bass sound
    
    ADSR.trigger=0; //resets trigger value
    
    
    
    
    
    
    //Synth and guitar synthesis

    //variables to control parameters
    float duty;
    float synthInput; //volume
    
    if(guitarPlay == false) //iconic synth part
    {
        duty = 0.6;
        synthInput = 0.1;
        
        if(synthPart == 0)
        {
            if(sixteenClock.tick && stepCount == 2)
            {
                synthFreq = FSharp2; //Frequency has been calculated through google and are assigned to notes as a global variable;
                ADSR1.trigger = 1;
            }
            
            if(sixteenClock.tick && stepCount == 8)
            {
                synthFreq = AFlat;
                ADSR1.trigger = 1;
            }
        }
        if(synthPart == 1)
        {
            if(sixteenClock.tick && stepCount == 2)
            {
                synthFreq = A2;
                ADSR1.trigger = 1;
            }
            
            if(sixteenClock.tick && stepCount == 8)
            {
                synthFreq = AFlat;
                ADSR1.trigger = 1;
            }
        }
    }
       
    
    if(guitarPlay == true) //Inspired by iconic guitar riff, plucky synth with heavy bass sidechain effect
        {
            duty = 1;
            synthInput = 0.3;
            
            if(sixteenClock.tick)
            {
                
                if(stepCount == 2)
                {
                    synthFreq = CSharp2;
                    ADSR1.trigger = 1;
                }
                if(stepCount == 4)
                {
                    ADSR1.trigger = 1;
                }
                if(stepCount == 5)
                {
                    synthFreq = B2;
                    ADSR1.trigger = 1;
                }
                if(stepCount == 7)
                {
                    synthFreq = A3;
                    ADSR1.trigger = 1;
                }
                if(stepCount == 9)
                {
                    synthFreq = CSharp2;
                    ADSR1.trigger = 1;
                }
            }
        }
    
    
    ADSR1out=ADSR1.adsr(synthInput,ADSR1.trigger); //volume adjusts at guitar part
    LFO3out=LFO3.sinebuf(0.2);
    VCO3out=VCO3.pulse((synthFreq / 2),duty); //waveform adjusts when guitar part is played
    VCO2out=VCO4.pulse(synthFreq / 2,0.2);
    VCFout=VCF.lores((VCO3out+VCO4out)*0.3, ADSR1out*10000, 2);
    double synthSound= VCF1out * ADSR1out;
   
    ADSR1.trigger=0; //resets trigger
    
    
    
    
    
    
    //Drums - this is a different way of playing samples, kick has been triggered in another function and is played here and assigned to a variable
    double drumSound;
    
        //Starts samples
         if(startKick == true)
         {
             double drumSound = playDrum(kickSpeed); //kickspeed = 1;
         }
     
         if(startSnare == true)
         {
             drumSound = playDrum(snareSpeed); //snarespeed = 2.5;
         }
   
    
    
    double finalOut; //variable that is used to create the finaloutput
    
   
    if(stopPlaying == false)
   {
       finalOut = drumSound + bassSound + synthSound + billieJeanSampleSignal + theOneSampleSignal + tellMeSampleSignal + moreLikeSampleSignal; //output mixes together all elements. This contains the drum, bass and synth sound which have been created in this function, as well as the signal of all the samples. They have been assigned seperate variables so that they can play at the same time
   }else
   
   {
       finalOut = billieJeanSampleSignal;
   }
    
    
   
    
    output[0] = finalOut; //sending the final mix to the speakers
    output[1] = output[0];
    
}


