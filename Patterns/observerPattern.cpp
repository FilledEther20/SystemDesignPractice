#include<iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class ISubscriber{
public:
    virtual void update()=0;
    virtual ~ISubscriber(){};
};

// Abstract Observable interface: a YouTube channel interface
class IChannel{
public:
    virtual void subscribe(ISubscriber* subscriber)=0;
    virtual void unsubscribe(ISubscriber* subscriber) = 0;
    virtual void notifySubscribers() = 0;
    virtual ~IChannel() {};
};

class Channel:public IChannel{
private:
    vector<ISubscriber*> subscribers;
    string name;
    string latestVideo;
    
public:
    Channel(const string &name){
        this->name=name;
    }

    // Add a subscriber (avoid duplicates)
    void subscribe(ISubscriber *subscriber) override{
        if(find(subscribers.begin(),subscribers.end(),subscriber)==subscribers.end()){
            subscribers.push_back(subscriber);
        }        
    }

    // Remove a subscriber if present
    void unsubscribe(ISubscriber *subscriber) override{
        auto it = find(subscribers.begin(), subscribers.end(), subscriber);
        if (it != subscribers.end()) {
            subscribers.erase(it);
        }
    }

    // Notify all subscribers of the latest video
    void notifySubscribers() override{
        for(ISubscriber *sub:subscribers){
            sub->update();
        }
    }

    // Upload a new video and notify all subscribers
    void uploadVideo(const string& title) {
        latestVideo = title;
        cout << name << " uploaded " << title;
        notifySubscribers();
    }

    // Read video data
    string getVideoData() {
        return "Checkout our new Video : " + latestVideo;
    }
};

class Subscriber : public ISubscriber {
private:
    string name;
    Channel* channel;
public:
    Subscriber(const string& name, Channel* channel) {
        this->name = name;
        this->channel = channel;
    }

    void update() override {
        cout << "Hey " << name << "," << this->channel->getVideoData();
    }
};


int main(){
    
    Channel* channel=new Channel("Temp");
    Subscriber* subs1=new Subscriber("Varun",channel); 
    Subscriber * subs2=new Subscriber("Tarun",channel);

    channel->subscribe(subs1);
    channel->subscribe(subs2);

    channel->uploadVideo("Observer Tutorial");

    channel->unsubscribe(subs1);

    channel->uploadVideo("Decorator Pattern Tutorial");
    return 0;
}