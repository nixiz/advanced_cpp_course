#pragma once
#include <playground_organizer.hpp>
#include <vector>
#include <set>
#include <memory>
#include "inc/number.hpp"

namespace weakpointer {

  namespace problem {

    namespace definition {
      class TopicPublisher {
      public:
        class Observer {
          TopicPublisher *_topicPtr;
          bool disabledUnregister;
        public:
          Observer(TopicPublisher *topic) : _topicPtr(topic), disabledUnregister(false) {
            _topicPtr->RegisterObserver(this);
          }          
          virtual ~Observer()
          {
            // hatayý gerçekleþtirebilmek için
            if (!disabledUnregister)
            {
              _topicPtr->UnregisterObserver(this);
            }
          }
        public:
          virtual void OnTopicUpdated(std::string) = 0;
          Observer* DisableUnregistering() {
            disabledUnregister = true;
            return this;
          }
        };
      public:
        void UpdateTopic(std::string newTopic) {
          for (Observer* observer : observers) {
            observer->OnTopicUpdated(newTopic);
          }
        }

      private:
        friend class Observer;
        std::set<Observer*> observers;
        void RegisterObserver(Observer* observer) {
          observers.insert(observer);
        }
        void UnregisterObserver(Observer* observer) {
          observers.erase(observer);
        }
      };

      class TopicSubscriber : public TopicPublisher::Observer {
      public:
        TopicSubscriber(TopicPublisher *publisher) : TopicPublisher::Observer(publisher) { }
      private:
        virtual void OnTopicUpdated(std::string newTopic) {
          std::cout << newTopic << std::endl;
        }
      };

      class ProblemDemo {
      public:
        static void Demonstrate() {
          TopicPublisher topic;
          std::vector<TopicSubscriber*> subscribers;
          for (size_t i = 0; i < 5; i++)
          {
            subscribers.push_back(new TopicSubscriber(&topic));
          }
          topic.UpdateTopic("ProblemDemo::Demonstrate()");

          // destroy random subscriber in time
          delete subscribers[3]->DisableUnregistering();

          /*
          int *ptr = new int(10);
          int *ptrDup = ptr;
          
          delete ptr;
          // ptrDup still points to the memory location pointed by ptr which no longer exists. 
          // So, deferencing ptrDup results undefined behavior. 
          */
          
          // enable below line will cause undefined behaivor for deleted observer.
          //topic.UpdateTopic("will crash application");
        }
      };
    } // namespace problem::definition

    namespace sharedptr {

      class TopicPublisher {
      public:
        class Observer : public std::enable_shared_from_this<Observer> {
          TopicPublisher *_topicPtr;
        public:
          Observer(TopicPublisher *topic) : _topicPtr(topic) {
            // cannot shared_from_this here because inner weak_ptr still not generated
            //_topicPtr->RegisterObserver(shared_from_this());
          }
          virtual ~Observer() {
            std::cout << "~Observer will never call" << std::endl;
          }
        public:
          virtual void OnTopicUpdated(std::string) = 0;
          void Register() {
            _topicPtr->RegisterObserver(shared_from_this());
          }
          void Unregister() {
            _topicPtr->UnregisterObserver(shared_from_this());
          }
        };
      public:
        void UpdateTopic(std::string newTopic) {
          for (auto observer : observers) {
            observer->OnTopicUpdated(newTopic);
          }
        }

      private:
        friend class Observer;
        std::set<std::shared_ptr<Observer>> observers;
        void RegisterObserver(std::shared_ptr<Observer> observer) {
          observers.insert(observer);
        }
        void UnregisterObserver(std::shared_ptr<Observer> observer) {
          observers.erase(observer);
        }
      };

      class TopicSubscriber : public TopicPublisher::Observer {
      public:
        TopicSubscriber(TopicPublisher *publisher) : TopicPublisher::Observer(publisher) { }
      private:
        virtual void OnTopicUpdated(std::string newTopic) {
          std::cout << newTopic << std::endl;
        }
      };

      class ProblemDemo {
      public:
        static void Demonstrate() {
          TopicPublisher topic;
          std::vector<std::shared_ptr<TopicSubscriber>> subscribers;
          for (size_t i = 0; i < 5; i++)
          {
            auto subs = std::make_shared<TopicSubscriber>(&topic);
            subs->Register();
            subscribers.push_back(subs);
          }
          topic.UpdateTopic("new topic 2");

          // destroy random subscriber in time
          subscribers.clear();

          topic.UpdateTopic("subscribers are alive!!");
        }
      };
    } // namespace problem::sharedptr

  } // namespace problem 

  namespace solution {

    class TopicPublisher {
    public:
      class Observer : public std::enable_shared_from_this<Observer> {
        std::weak_ptr<TopicPublisher> topicPtr;
      public:
        Observer(std::weak_ptr<TopicPublisher> topic) : topicPtr(topic) { }
        virtual ~Observer() {
          std::cout << "~Observers destructed successfully" << std::endl;
        }
      public:
        virtual void OnTopicUpdated(std::string) = 0;
        void Register() {
          if (topicPtr.expired()) return;
          topicPtr.lock()->RegisterObserver(shared_from_this());
        }
        void Unregister() {
          if (topicPtr.expired()) return;
          topicPtr.lock()->UnregisterObserver(shared_from_this());
        }
      };
    public:
      void UpdateTopic(std::string newTopic) {
        for (const std::weak_ptr<Observer>& observer : observers) {
          if (observer.expired()) continue;

          std::shared_ptr<Observer> observer_locked = observer.lock();
          observer_locked->OnTopicUpdated(newTopic);
        }
      }

    private:
      friend class Observer;
      std::set<std::weak_ptr<Observer>, std::owner_less<std::weak_ptr<Observer>>> observers;
      void RegisterObserver(std::weak_ptr<Observer> observer) {
        observers.insert(observer);
      }
      void UnregisterObserver(std::weak_ptr<Observer> observer) {
        observers.erase(observer);
      }
    };

    class TopicSubscriber : public TopicPublisher::Observer {
    public:
      TopicSubscriber(std::weak_ptr<TopicPublisher> publisher) : TopicPublisher::Observer(publisher) { }
    private:
      virtual void OnTopicUpdated(std::string newTopic) {
        std::cout << newTopic << std::endl;
      }
    };

    class SolutionDemo {
    public:
      static void Demonstrate() {
        std::shared_ptr<TopicPublisher> topic(new TopicPublisher());
        std::vector<std::shared_ptr<TopicSubscriber>> subscribers;
        for (size_t i = 0; i < 5; i++)
        {
          auto subs = std::make_shared<TopicSubscriber>(topic);
          subs->Register();
          subscribers.push_back(subs);
        }
        topic->UpdateTopic("new topic 2");

        // destroy random subscriber in time
        subscribers.clear();

        topic->UpdateTopic("there are no subscribers");
      }
    };

  } // namespace solution 

} // namespace weakpointer

CREATE_ELEMENT_WITH_CODE(WeakPointerUsageRealWorld) {
  using namespace weakpointer;
  problem::definition::ProblemDemo::Demonstrate();
  problem::sharedptr::ProblemDemo::Demonstrate();
  solution::SolutionDemo::Demonstrate();
}