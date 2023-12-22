//
// Created by keren on 09/05/2023.
//

#include "Movie.h"

double Movie::get_rating() const{
    if (sumRatings == 0)
        return 0;
    return (double)((double)sumRatings / (double)numRatings);
}

int Movie::get_views() const {
    return views;
}

int Movie::get_id() const {
    return movieId;
}

Genre Movie::get_genre() const{
    return genre;
}

bool Movie::is_vip() const {
    return vipOnly;
}

void Movie::submit_rating(int rating) {
    numRatings++;
    sumRatings += rating;
}

void Movie::add_views(int v) {
    views += v;
}

