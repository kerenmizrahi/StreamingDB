

#ifndef DATA_STRUCTURES_2_MOVIE_H
#define DATA_STRUCTURES_2_MOVIE_H

#include "wet1util.h"

class Movie {
private:
    int movieId;
    Genre genre;
    int views;
    bool vipOnly;
    int sumRatings;
    int numRatings;

public:
    Movie(int movieId, Genre genre, int views, bool vipOnly)
         : movieId(movieId), genre(genre), views(views), vipOnly(vipOnly), sumRatings(0), numRatings(0) {};
    double get_rating() const;
    int get_views() const;
    int get_id() const;
    Genre get_genre() const;
    bool is_vip() const;
    void submit_rating(int rating);
    void add_views(int v);
};



#endif //DATA_STRUCTURES_2_MOVIE_H
