

#ifndef DATA_STRUCTURES_2_FUNCTOR_H
#define DATA_STRUCTURES_2_FUNCTOR_H

#include "Group.h"
#include "Movie.h"

class UserById {
public:
    bool operator()(std::shared_ptr<User> u1, std::shared_ptr<User> u2){
        return u1->get_id() < u2->get_id();
    }
};

class GroupById {
public:
    bool operator()(std::shared_ptr<Group> g1, std::shared_ptr<Group> g2){
        return g1->get_id() < g2->get_id();
    }
};

class MovieById {
public:
    bool operator()(std::shared_ptr<Movie> m1, std::shared_ptr<Movie> m2){
        return m1->get_id() < m2->get_id();
    }
};

class MovieByRating {
public:
    bool operator()(std::shared_ptr<Movie> m1, std::shared_ptr<Movie> m2) {
        if (m1->get_rating() < m2->get_rating())
            return true;

        else if (m1->get_rating() == m2->get_rating()){
            if (m1->get_views() < m2->get_views())
                return true;

            else if (m1->get_views() == m2->get_views()){
                return m1->get_id() > m2->get_id();
            }
        }
        return false;
    }
};

#endif //DATA_STRUCTURES_2_FUNCTOR_H
