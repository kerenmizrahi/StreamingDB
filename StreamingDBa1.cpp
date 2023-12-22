#include "StreamingDBa1.h"

template <class Cond>
void delete_tree(std::shared_ptr<Node<std::shared_ptr<Movie>, Cond>>& node);
void delete_tree(std::shared_ptr<Node<std::shared_ptr<Group>, GroupById>>& node);
void delete_tree(std::shared_ptr<Node<std::shared_ptr<User>, UserById>>& node); /// הורדנו סימן רפרנס
void print2D(std::shared_ptr<Node<std::shared_ptr<Movie>, MovieByRating>> root, int space);
void print2D(std::shared_ptr<Node<std::shared_ptr<Movie>, MovieById>> root, int space);
void print2D(std::shared_ptr<Node<std::shared_ptr<User>, UserById>> root, int space);

std::shared_ptr<Node<std::shared_ptr<Group>, GroupById>> find_group(std::shared_ptr<Node<std::shared_ptr<Group>, GroupById>> root, int id);
std::shared_ptr<Node<std::shared_ptr<Movie>, MovieById>> find_movie_by_id(std::shared_ptr<Node<std::shared_ptr<Movie>, MovieById>> root, int id);
std::shared_ptr<Node<std::shared_ptr<Movie>, MovieByRating>> find_movie_by_rating(std::shared_ptr<Node<std::shared_ptr<Movie>, MovieByRating>> root, int id);
std::shared_ptr<Node<std::shared_ptr<User>, UserById>> find_user(std::shared_ptr<Node<std::shared_ptr<User>, UserById>> root, int id);
void printInorder(std::shared_ptr<Node<std::shared_ptr<Movie>, MovieByRating>> root, int * const output);

streaming_database::streaming_database()
{
    totalDrama = 0;
    totalComedy = 0;
    totalAction = 0;
    totalFantasy = 0;
    topDrama = 0;
    topAction = 0;
    topComedy = 0;
    topFantasy = 0;
}

streaming_database::~streaming_database()
{
    delete_tree(groups.r);
    delete_tree(users.r);
    delete_tree(moviesById.r);
    delete_tree(moviesByRatings.r);
    delete_tree(comedyMovies.r);
    delete_tree(dramaMovies.r);
    delete_tree(actionMovies.r);
    delete_tree(fantasyMovies.r);
}

StatusType streaming_database::add_movie(int movieId, Genre genre, int views, bool vipOnly)
{
    if (movieId <= 0 || genre == Genre::NONE || views < 0)
        return StatusType::INVALID_INPUT;

    try {
        if (find_movie_by_id(moviesById.r, movieId))
            return StatusType::FAILURE;

        std::shared_ptr<Movie> movie_to_add(new Movie(movieId, genre, views, vipOnly));
        Node<std::shared_ptr<Movie>, MovieById> add_by_id(movie_to_add);
        Node<std::shared_ptr<Movie>, MovieByRating> add_by_rating(movie_to_add);

        //std::cout << "before insert: " << std::endl;
        //print2D(moviesByRatings.r, 5);

        moviesById.r = moviesById.insert(moviesById.r, add_by_id);
        moviesByRatings.r = moviesByRatings.insert(moviesByRatings.r, add_by_rating);

        //std::cout << "after insert: " << std::endl;
        //print2D(moviesByRatings.r, 5);

        switch (genre) {
            case Genre::COMEDY:
                comedyMovies.r = comedyMovies.insert(comedyMovies.r, add_by_rating);
                topComedy = comedyMovies.maxValueNode(comedyMovies.r)->value->get_id();
                totalComedy++;
                break;

            case Genre::DRAMA:
                dramaMovies.r = dramaMovies.insert(dramaMovies.r, add_by_rating);
                topDrama = dramaMovies.maxValueNode(dramaMovies.r)->value->get_id();
                totalDrama++;
                break;

            case Genre::ACTION:
                actionMovies.r = actionMovies.insert(actionMovies.r, add_by_rating);
                topAction = actionMovies.maxValueNode(actionMovies.r)->value->get_id();
                totalAction++;
                break;

            default:
                fantasyMovies.r = fantasyMovies.insert(fantasyMovies.r, add_by_rating);
                topFantasy = fantasyMovies.maxValueNode(fantasyMovies.r)->value->get_id();
                totalFantasy++;
        }


    }
    catch(std::exception& e){
        return  StatusType::ALLOCATION_ERROR;
    }

    return StatusType::SUCCESS;
}

StatusType streaming_database::remove_movie(int movieId)
{
    if (movieId <= 0)
        return StatusType::INVALID_INPUT;

    try{
        std::shared_ptr<Node<std::shared_ptr<Movie>, MovieById>> movie_to_remove = find_movie_by_id(moviesById.r, movieId);
        if (movie_to_remove == nullptr)
            return StatusType::FAILURE;

        switch (movie_to_remove->value->get_genre()){
            case Genre::COMEDY:
                comedyMovies.r = comedyMovies.deleteNode(comedyMovies.r, movie_to_remove->value);
                if (comedyMovies.r == nullptr)
                    topComedy = 0;
                else
                    topComedy = comedyMovies.maxValueNode(comedyMovies.r)->value->get_id();
                totalComedy--;
                break;

            case Genre::DRAMA:
                dramaMovies.r = dramaMovies.deleteNode(dramaMovies.r, movie_to_remove->value);
                if (dramaMovies.r == nullptr)
                    topDrama = 0;
                else
                    topDrama = dramaMovies.maxValueNode(dramaMovies.r)->value->get_id();
                totalDrama--;
                break;

            case Genre::ACTION:
                actionMovies.r = actionMovies.deleteNode(actionMovies.r, movie_to_remove->value);
                if (actionMovies.r == nullptr)
                    topAction = 0;
                else
                    topAction = actionMovies.maxValueNode(actionMovies.r)->value->get_id();
                totalAction--;
                break;

            default:
                fantasyMovies.r = fantasyMovies.deleteNode(fantasyMovies.r, movie_to_remove->value);
                if (fantasyMovies.r == nullptr)
                    topFantasy = 0;
                else
                    topFantasy = fantasyMovies.maxValueNode(fantasyMovies.r)->value->get_id();
                totalFantasy--;
        }

        moviesByRatings.r = moviesByRatings.deleteNode(moviesByRatings.r, movie_to_remove->value);
        moviesById.r = moviesById.deleteNode(moviesById.r, movie_to_remove->value);
    }
    catch (std::exception& e) {
        return StatusType::ALLOCATION_ERROR;
    }

    return StatusType::SUCCESS;
}

StatusType streaming_database::add_user(int userId, bool isVip)
{
    if (userId <= 0)
        return StatusType::INVALID_INPUT;
    try{
        if (find_user(users.r, userId))
            return StatusType::FAILURE;

        std::shared_ptr<User> user_to_add(new User(userId, isVip));
        Node<std::shared_ptr<User>, UserById> add_by_id(user_to_add);

        users.r = users.insert(users.r, add_by_id);

    } catch(std::exception& e){
        return  StatusType::ALLOCATION_ERROR;
    }

    return StatusType::SUCCESS;
}

StatusType streaming_database::remove_user(int userId)
{
    if (userId <= 0)
        return StatusType::INVALID_INPUT;

    try{
        std::shared_ptr<Node<std::shared_ptr<User>, UserById>> user_to_remove = find_user(users.r, userId);
        if (user_to_remove == nullptr)
            return StatusType::FAILURE;

        if (user_to_remove -> value -> get_group() != nullptr)   /// the user is in a group
        {
            std::shared_ptr<Group> user_group = user_to_remove->value->get_group();

            /// remove user from members tree
            user_group->get_members().r = user_group->get_members().deleteNode(user_group->get_members().r, user_to_remove->value);
            //print2D(user_group->get_members().r, 5);

            /// update sum_<g> in users group
            int user_action_views = get_num_views(userId, Genre::ACTION).ans();
            user_group->set_action_sum(user_group->get_action_sum() - user_action_views);
            int user_comedy_views = get_num_views(userId, Genre::COMEDY).ans();
            user_group->set_comedy_sum(user_group->get_comedy_sum() - user_comedy_views);
            int user_fantasy_views = get_num_views(userId, Genre::FANTASY).ans();
            user_group->set_fantasy_sum(user_group->get_fantasy_sum() - user_fantasy_views);
            int user_drama_views = get_num_views(userId, Genre::DRAMA).ans();
            user_group->set_drama_sum(user_group->get_drama_sum() - user_drama_views);
            /// reduce 1 from total members
            user_group -> reduce_total_members();
            /// check if user is vip
            if (user_to_remove->value->is_vip())
                user_group->reduce_sum_vip();
        }
        /// remove user from all dataBase
        users.r = users.deleteNode(users.r, user_to_remove->value);

    } catch(std::exception& e){
        return  StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;
}

StatusType streaming_database::add_group(int groupId)
{
    if (groupId <= 0)
        return StatusType::INVALID_INPUT;
    try {
        if (find_group(groups.r, groupId))
            return StatusType::FAILURE;

        std::shared_ptr<Group> group_to_add(new Group(groupId));
        Node<std::shared_ptr<Group>, GroupById> add_by_id(group_to_add);

        groups.r = groups.insert(groups.r, add_by_id);

    } catch(std::exception& e){
        return  StatusType::ALLOCATION_ERROR;
    }

    return StatusType::SUCCESS;
}

StatusType streaming_database::remove_group(int groupId)
{
    if (groupId <= 0)
        return StatusType::INVALID_INPUT;

    try{
        std::shared_ptr<Node<std::shared_ptr<Group>, GroupById>> group_to_remove = find_group(groups.r, groupId);
        if (group_to_remove == nullptr)
            return StatusType::FAILURE;

        group_to_remove->value->delete_tree(group_to_remove->value->get_members().r);
        groups.r = groups.deleteNode(groups.r, group_to_remove->value);

    } catch(std::exception& e){
        return  StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;
}

StatusType streaming_database::add_user_to_group(int userId, int groupId)
{
    if (userId <= 0 || groupId <= 0)
        return StatusType::INVALID_INPUT;

    try{
        std::shared_ptr<Node<std::shared_ptr<Group>, GroupById>> group = find_group(groups.r, groupId);
        std::shared_ptr<Node<std::shared_ptr<User>, UserById>> user = find_user(users.r, userId);
        if (user == nullptr || group == nullptr)
            return StatusType::FAILURE;

        if (user->value->get_group() != nullptr) {
            //if (user->value->get_group()->get_id() == groupId)
            //    return StatusType::SUCCESS;
            //else
            return StatusType::FAILURE;
        }

        group->value->add_member(user->value->get_action_views(), user->value->get_comedy_views(), user->value->get_drama_views(), user->value->get_fantasy_views());
        if (user->value->is_vip())
            group->value->add_sum_vip();

        user->value->init_fix(group->value->get_action_views(), group->value->get_comedy_views(), group->value->get_drama_views(), group->value->get_fantasy_views());
        user->value->set_group(group->value);

        group->value->get_members().r = group->value->get_members().insert(group->value->get_members().r, user->value);
        //print2D(group->value->get_members().r, 5);
    }
    catch (std::exception& e){
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;
}

StatusType streaming_database::user_watch(int userId, int movieId) {
    if (userId <= 0 || movieId <= 0)
        return StatusType::INVALID_INPUT;

    try {
        std::shared_ptr<Node<std::shared_ptr<User>, UserById>> user = find_user(users.r, userId);
        std::shared_ptr<Node<std::shared_ptr<Movie>, MovieById>> movie_to_watch = find_movie_by_id(moviesById.r, movieId);

        if (user == nullptr || movie_to_watch == nullptr)
            return StatusType::FAILURE;
        if (movie_to_watch->value->is_vip())
            if (!user->value->is_vip())
                return StatusType::FAILURE;
        /// update moviesByRatings tree
        moviesByRatings.r = moviesByRatings.deleteNode(moviesByRatings.r, movie_to_watch->value);


        /// update user genre view + top update
        Genre g = movie_to_watch->value->get_genre();
        switch(g) {
            case Genre::DRAMA :
                user->value->add_drama_views(1);
                dramaMovies.r = dramaMovies.deleteNode(dramaMovies.r, movie_to_watch->value);
                movie_to_watch->value->add_views(1);
                dramaMovies.r = dramaMovies.insert(dramaMovies.r, movie_to_watch->value);
                topDrama = dramaMovies.maxValueNode(dramaMovies.r)->value->get_id();
                break;

            case Genre::COMEDY :
                user->value->add_comedy_views(1);
                comedyMovies.r = comedyMovies.deleteNode(comedyMovies.r, movie_to_watch->value);
                movie_to_watch->value->add_views(1);
                comedyMovies.r = comedyMovies.insert(comedyMovies.r, movie_to_watch->value);
                topComedy = comedyMovies.maxValueNode(comedyMovies.r)->value->get_id();
                break;

            case Genre::FANTASY :
                user->value->add_fantasy_views(1);
                fantasyMovies.r = fantasyMovies.deleteNode(fantasyMovies.r, movie_to_watch->value);
                movie_to_watch->value->add_views(1);
                fantasyMovies.r = fantasyMovies.insert(fantasyMovies.r, movie_to_watch->value);
                topFantasy = fantasyMovies.maxValueNode(fantasyMovies.r)->value->get_id();
                break;

            case Genre::ACTION :
                user->value->add_action_views(1);
                actionMovies.r = actionMovies.deleteNode(actionMovies.r, movie_to_watch->value);
                movie_to_watch->value->add_views(1);
                actionMovies.r = actionMovies.insert(actionMovies.r, movie_to_watch->value);
                topAction = actionMovies.maxValueNode(actionMovies.r)->value->get_id();
                break;

            default:
                break;
        }
        moviesByRatings.r = moviesByRatings.insert(moviesByRatings.r, movie_to_watch->value);
        /// is user is in group
        if(user->value->get_group() != nullptr) {
            std::shared_ptr<Group> user_group = user->value->get_group();
            switch (g) {
                case Genre::DRAMA :
                    user_group->set_drama_sum(user_group -> get_drama_sum() + 1);
                    break;
                case Genre::COMEDY :
                    user_group->set_comedy_sum(user_group -> get_comedy_sum() + 1);
                    break;
                case Genre::FANTASY :
                    user_group->set_fantasy_sum(user_group -> get_fantasy_sum() + 1);
                    break;
                case Genre::ACTION :
                    user_group->set_action_sum(user_group -> get_action_sum() + 1);
                    break;
                default:
                    break;
            }
        }
    }
    catch (std::exception &e) {
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;
}

StatusType streaming_database::group_watch(int groupId, int movieId)
{
    if (movieId <= 0 || groupId <= 0)
        return StatusType::INVALID_INPUT;

    try{
        std::shared_ptr<Node<std::shared_ptr<Group>, GroupById>> group = find_group(groups.r, groupId);
        std::shared_ptr<Node<std::shared_ptr<Movie> ,MovieById>> movie = find_movie_by_id(moviesById.r, movieId);
        if (movie == nullptr || group == nullptr || group->value->get_total_members() == 0 || (movie->value->is_vip() && !group->value->is_vip()))
            return StatusType::FAILURE;

        moviesByRatings.r = moviesByRatings.deleteNode(moviesByRatings.r, movie->value);

        switch(movie->value->get_genre()){
            case Genre::DRAMA :
                dramaMovies.r = dramaMovies.deleteNode(dramaMovies.r, movie->value);
                movie->value->add_views(group->value->get_total_members());
                group->value->set_drama_sum(group->value->get_drama_sum() + group->value->get_total_members());
                group->value->add_drama_views(1);
                dramaMovies.r = dramaMovies.insert(dramaMovies.r, movie->value);
                topDrama = dramaMovies.maxValueNode(dramaMovies.r)->value->get_id();
                break;

            case Genre::COMEDY :
                comedyMovies.r = comedyMovies.deleteNode(comedyMovies.r, movie->value);
                movie->value->add_views(group->value->get_total_members());
                group->value->set_comedy_sum(group->value->get_comedy_sum() + group->value->get_total_members());
                group->value->add_comedy_views(1);
                comedyMovies.r = comedyMovies.insert(comedyMovies.r, movie->value);
                topComedy = comedyMovies.maxValueNode(comedyMovies.r)->value->get_id();
                break;

            case Genre::ACTION :
                actionMovies.r = actionMovies.deleteNode(actionMovies.r, movie->value);
                movie->value->add_views(group->value->get_total_members());
                group->value->set_action_sum(group->value->get_action_sum() + group->value->get_total_members());
                group->value->add_action_views(1);
                actionMovies.r = actionMovies.insert(actionMovies.r, movie->value);
                topAction = actionMovies.maxValueNode(actionMovies.r)->value->get_id();
                break;

            case Genre::FANTASY :
                fantasyMovies.r = fantasyMovies.deleteNode(fantasyMovies.r, movie->value);
                movie->value->add_views(group->value->get_total_members());
                group->value->set_fantasy_sum(group->value->get_fantasy_sum() + group->value->get_total_members());
                group->value->add_fantasy_views(1);
                fantasyMovies.r = fantasyMovies.insert(fantasyMovies.r, movie->value);
                topFantasy = fantasyMovies.maxValueNode(fantasyMovies.r)->value->get_id();

            default:
                break;
        }

        moviesByRatings.r = moviesByRatings.insert(moviesByRatings.r, movie->value);
    }
    catch (std::exception& e){
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;
}

output_t<int> streaming_database::get_all_movies_count(Genre genre)
{
    switch(genre){
        case Genre::DRAMA : return totalDrama;
        case Genre::COMEDY : return totalComedy;
        case Genre::FANTASY : return totalFantasy;
        case Genre::ACTION : return totalAction;
        default : return (totalDrama + totalComedy + totalAction + totalFantasy);
    }
}

StatusType streaming_database::get_all_movies(Genre genre, int *const output)
{
    if (output == nullptr)
        return StatusType::INVALID_INPUT;

    int *movies_array;
    try {
        //--------------------- printing ---------------------------------------------
        //std::cout << "movies by ratings: " << std::endl;
        //print2D(moviesByRatings.r, 5);

        int arr_size = get_all_movies_count(genre).ans();
        if (arr_size == 0)
            return StatusType::FAILURE;

        movies_array = new int[arr_size];

        switch (genre) {
            case Genre::DRAMA :
                printInorder(dramaMovies.r, movies_array);
                break;
            case Genre::COMEDY :
                printInorder(comedyMovies.r, movies_array);
                break;
            case Genre::ACTION :
                printInorder(actionMovies.r, movies_array);
                break;
            case Genre::FANTASY :
                printInorder(fantasyMovies.r, movies_array);
                break;
            default:
                printInorder(moviesByRatings.r, movies_array);
        }

        for (int i = 0; i < arr_size; ++i) {
            output[i] = movies_array[arr_size - i - 1];
        }
    }
    catch (std::exception& e){
        delete[] movies_array;
        return StatusType::ALLOCATION_ERROR;
    }

    delete[] movies_array;
    return StatusType::SUCCESS;
}

output_t<int> streaming_database::get_num_views(int userId, Genre genre) {
    if (userId <= 0)
        return StatusType::INVALID_INPUT;
    try {
        std::shared_ptr<Node<std::shared_ptr<User>, UserById>> user = find_user(users.r, userId);
        if (user == nullptr)
            return StatusType::FAILURE;

        if (genre == Genre::NONE) {
            /// user is in a group
            if (user->value->get_group() != nullptr) {
                int sum = user->value->get_group()->sum_of_group_views();
                sum += user->value->sum_of_fix_views();
                return sum;
            }
                /// user is not in group
            else
                return user->value->sum_of_views();
        }
        else
        {
            /// user is in a group
            if (user->value->get_group() != nullptr)
            {
                switch (genre) {
                    case Genre::DRAMA :
                        return user->value->get_group()->get_drama_views() - user->value->get_drama_fix() +
                               user->value->get_drama_views();
                    case Genre::COMEDY :
                        return user->value->get_group()->get_comedy_views() - user->value->get_comedy_fix() +
                               user->value->get_comedy_views();
                    case Genre::FANTASY :
                        return user->value->get_group()->get_fantasy_views() - user->value->get_fantasy_fix() +
                               user->value->get_fantasy_views();
                    case Genre::ACTION :
                        return user->value->get_group()->get_action_views() - user->value->get_action_fix() +
                               user->value->get_action_views();
                    default:
                        break;
                }
            }
            else
            {
                switch (genre) {
                    case Genre::DRAMA :
                        return user->value->get_drama_views();
                    case Genre::COMEDY :
                        return user->value->get_comedy_views();
                    case Genre::FANTASY :
                        return user->value->get_fantasy_views();
                    case Genre::ACTION :
                        return user->value->get_action_views();
                    default:
                        break;
                }
            }
        }
    }
    catch (std::exception & e) {
        return StatusType::ALLOCATION_ERROR;
    }

    return StatusType::SUCCESS;
}

StatusType streaming_database::rate_movie(int userId, int movieId, int rating)
{
    if (userId <= 0 || movieId <= 0 || rating < 0 || rating > 100)
        return StatusType::INVALID_INPUT;

    try {
        std::shared_ptr<Node<std::shared_ptr<User>, UserById>> user = find_user(users.r, userId);
        std::shared_ptr<Node<std::shared_ptr<Movie>, MovieById>> movie_to_rate = find_movie_by_id(moviesById.r, movieId);
        if (user == nullptr || movie_to_rate == nullptr)
            return StatusType::FAILURE;

        if (movie_to_rate->value->is_vip())
            if (!user->value->is_vip())
                return StatusType::FAILURE;

        moviesByRatings.r = moviesByRatings.deleteNode(moviesByRatings.r, movie_to_rate->value);

        Genre g = movie_to_rate->value->get_genre();
        switch(g) {
            case Genre::DRAMA :
                dramaMovies.r = dramaMovies.deleteNode(dramaMovies.r, movie_to_rate->value);
                movie_to_rate->value->submit_rating(rating);
                dramaMovies.r = dramaMovies.insert(dramaMovies.r, movie_to_rate->value);
                topDrama = dramaMovies.maxValueNode(dramaMovies.r)->value->get_id();
                break;

            case Genre::COMEDY :
                comedyMovies.r = comedyMovies.deleteNode(comedyMovies.r, movie_to_rate->value);
                movie_to_rate->value->submit_rating(rating);
                comedyMovies.r = comedyMovies.insert(comedyMovies.r, movie_to_rate->value);
                topComedy = comedyMovies.maxValueNode(comedyMovies.r)->value->get_id();
                break;

            case Genre::FANTASY :
                fantasyMovies.r = fantasyMovies.deleteNode(fantasyMovies.r, movie_to_rate->value);
                movie_to_rate->value->submit_rating(rating);
                fantasyMovies.r = fantasyMovies.insert(fantasyMovies.r, movie_to_rate->value);
                topFantasy = fantasyMovies.maxValueNode(fantasyMovies.r)->value->get_id();
                break;

            case Genre::ACTION :
                actionMovies.r = actionMovies.deleteNode(actionMovies.r, movie_to_rate->value);
                movie_to_rate->value->submit_rating(rating);
                actionMovies.r = actionMovies.insert(actionMovies.r, movie_to_rate->value);
                topAction = actionMovies.maxValueNode(actionMovies.r)->value->get_id();
                break;

            default:
                break;
        }

        moviesByRatings.r = moviesByRatings.insert(moviesByRatings.r, movie_to_rate->value);
    }
    catch (std::exception &e) {
        return StatusType::ALLOCATION_ERROR;
    }

    return StatusType::SUCCESS;
}

output_t<int> streaming_database::get_group_recommendation(int groupId)
{
    if (groupId <= 0)
        return StatusType::INVALID_INPUT;

    try{
        std::shared_ptr<Node<std::shared_ptr<Group>, GroupById>> group = find_group(groups.r, groupId);
        if (group == nullptr || group->value->get_total_members() == 0)
            return StatusType::FAILURE;

        int g = group->value->favorite_genre();

        switch (g) {
            case 0:
                if (totalComedy == 0)
                    return StatusType::FAILURE;
                return topComedy;

            case 1:
                if (totalDrama == 0)
                    return StatusType::FAILURE;
                return topDrama;

            case 2:
                if (totalAction == 0)
                    return StatusType::FAILURE;
                return topAction;

            default:
                if (totalFantasy == 0)
                    return StatusType::FAILURE;
                return topFantasy;
        }
    }
    catch (std::exception &e) {
        return StatusType::ALLOCATION_ERROR;
    }
}

void delete_tree(std::shared_ptr<Node<std::shared_ptr<User>, UserById>>& node) {
    if (node) {
        delete_tree(node->left);
        delete_tree(node->right);
        node->right.reset();
        node->left.reset();
        node->value->get_group().reset();
        //node->value.reset(); /// לשים לב גם פה
        node.reset();
    }
}

template <class Cond>
void delete_tree(std::shared_ptr<Node<std::shared_ptr<Movie>, Cond>>& node){
    if (node){
        delete_tree(node->left);
        delete_tree(node->right);
        node->right.reset();
        node->left.reset();
        //node->value.reset();
        node.reset();
    }
}

void delete_tree(std::shared_ptr<Node<std::shared_ptr<Group>, GroupById>>& node) {
    if (node) {
        delete_tree(node->left);
        delete_tree(node->right);
        node->right.reset();
        node->left.reset();
        delete_tree(node->value->get_members().r);
        node.reset();
    }
}

std::shared_ptr<Node<std::shared_ptr<Group>, GroupById>> find_group(std::shared_ptr<Node<std::shared_ptr<Group>, GroupById>> root, int id){
    if (root == nullptr) {
        return nullptr;
    }

    if (root -> value -> get_id() < id) { //id > root id
        return find_group(root -> right, id);
    }
    else if (root -> value -> get_id() > id) {  //val > root -> value
        return find_group(root -> left, id);
    }
    else {
///duplicate values
        return root;
    }
}

std::shared_ptr<Node<std::shared_ptr<Movie>, MovieById>> find_movie_by_id(std::shared_ptr<Node<std::shared_ptr<Movie>, MovieById>> root, int id) {
    if (root == nullptr) {
        return nullptr;
    }

    if ( root -> value -> get_id() < id) { //id > root id
        return find_movie_by_id(root->right, id);
    }
    else if (root ->  value -> get_id() > id) {  // val > root -> value
        return find_movie_by_id(root->left, id);
    }
    else {
///duplicate values
        return root;
    }
}

std::shared_ptr<Node<std::shared_ptr<Movie>, MovieByRating>> find_movie_by_rating(std::shared_ptr<Node<std::shared_ptr<Movie>, MovieByRating>> root, int id) {
    if (root == nullptr) {
        return nullptr;
    }

    if ( root -> value -> get_id() < id) { //id > root id
        return find_movie_by_rating(root->right, id);
    }
    else if (root ->  value -> get_id() > id) {  // val > root -> value
        return find_movie_by_rating(root->left, id);
    }
    else {
//duplicate values
        return root;
    }
}

std::shared_ptr<Node<std::shared_ptr<User>, UserById>> find_user(std::shared_ptr<Node<std::shared_ptr<User>, UserById>> root, int id){
    if (root == nullptr) {
        return nullptr;
    }

    if (root -> value -> get_id() < id) { //id > root id
        return find_user(root -> right, id);
    }
    else if (root -> value -> get_id() > id) {  //val > root -> value
        return find_user(root -> left, id);
    }
    else {
//duplicate values
        return root;
    }
}

void printInorder(std::shared_ptr<Node<std::shared_ptr<Movie>, MovieByRating>> root, int * const output) {
    int index = 0;

    if (root == nullptr)
        return;

    std::shared_ptr<Node<std::shared_ptr<Movie>, MovieByRating>> node = root;
    std::shared_ptr<Node<std::shared_ptr<Movie>, MovieByRating>> pre;

    while (node != nullptr) {
        if (node->left == nullptr) {
            output[index++] = node->value->get_id();
            node = node->right;
        } else {
            pre = node->left;
            while (pre->right != nullptr && pre->right != node)
                pre = pre->right;

            if (pre->right == nullptr) {
                pre->right = node;
                node = node->left;
            }
            else {
                pre->right = nullptr;
                output[index++] = node->value->get_id();
                node = node->right;
            }
        }

    }
}

void print2D(std::shared_ptr<Node<std::shared_ptr<Movie>, MovieByRating>> root, int space) {
    if (root == nullptr) // Base case  1
        return;

    space += SPACE; // Increase distance between levels   2
    print2D(root -> right, space); // Process right child first 3
    std::cout << std::endl;

    for (int i = SPACE; i < space; i++) // 5
        std::cout << " "; // 5.1
    std::cout << root -> value->get_id() << "\n"; // 6
    print2D(root -> left, space); // Process left child  7
}

void print2D(std::shared_ptr<Node<std::shared_ptr<Movie>, MovieById>> root, int space) {
    if (root == nullptr) // Base case  1
        return;

    space += SPACE; // Increase distance between levels   2
    print2D(root -> right, space); // Process right child first 3
    std::cout << std::endl;

    for (int i = SPACE; i < space; i++) // 5
        std::cout << " "; // 5.1
    std::cout << root -> value->get_id() << "\n"; // 6
    print2D(root -> left, space); // Process left child  7
}

void print2D(std::shared_ptr<Node<std::shared_ptr<User>, UserById>> root, int space) {
    if (root == nullptr) // Base case  1
        return;

    space += SPACE; // Increase distance between levels   2
    print2D(root -> right, space); // Process right child first 3
    std::cout << std::endl;

    for (int i = SPACE; i < space; i++) // 5
        std::cout << " "; // 5.1
    std::cout << root -> value->get_id() << "\n"; // 6
    print2D(root -> left, space); // Process left child  7
}
