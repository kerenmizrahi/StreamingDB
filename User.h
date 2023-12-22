#ifndef DATA_STRUCTURES_2_USER_H
#define DATA_STRUCTURES_2_USER_H

#include "AVLTree.h"
class Group;

class User {
private:
    int userId;
    bool isVip;
    std::shared_ptr<Group> group;
    int actionViews;
    int comedyViews;
    int dramaViews;
    int fantasyViews;
    int action_fix;
    int comedy_fix;
    int drama_fix;
    int fantasy_fix;

public:
    User(int userId, bool isVip)
            : userId(userId), isVip(isVip), group(nullptr),
              actionViews(0), comedyViews(0), dramaViews(0), fantasyViews(0),
              action_fix(0), comedy_fix(0), drama_fix(0), fantasy_fix(0) {};
    ~User() { group = nullptr; };
    int get_id() const;
    bool is_vip() const;
    void set_group(std::shared_ptr<Group> g);
    std::shared_ptr<Group> get_group() const;
    void init_fix(int av, int cv, int dv, int fv);
    void add_action_views(int v);
    void add_comedy_views(int v);
    void add_drama_views(int v);
    void add_fantasy_views(int v);
    int get_action_views() const;
    int get_comedy_views() const;
    int get_drama_views() const;
    int get_fantasy_views() const;
    int sum_of_fix_views() const;
    int sum_of_views() const;
    int get_drama_fix() const;
    int get_action_fix() const;
    int get_comedy_fix() const;
    int get_fantasy_fix() const;

    void views_after_removal(int av, int cv, int dv, int fv);

};


#endif //DATA_STRUCTURES_2_USER_H
