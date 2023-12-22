//
// Created by keren on 09/05/2023.
//

#ifndef DATA_STRUCTURES_2_GROUP_H
#define DATA_STRUCTURES_2_GROUP_H

#include "User.h"
class UserById;

class Group {
private:
    int groupId;
    int sumVip;
    AVLTree<std::shared_ptr<User>, UserById> members;
    int totalMembers;
    int actionViews;
    int comedyViews;
    int dramaViews;
    int fantasyViews;
    int action_sum;
    int comedy_sum;
    int drama_sum;
    int fantasy_sum;

public:
    Group(int groupId)
            : groupId(groupId) ,sumVip(0), members(), totalMembers(0),
              actionViews(0), comedyViews(0), dramaViews(0), fantasyViews(0),
              action_sum(0), comedy_sum(0), drama_sum(0), fantasy_sum(0){};
    ~Group() { delete_tree(members.r); };
    int get_id() const;
    bool is_vip() const;
    AVLTree<std::shared_ptr<User>, UserById>& get_members() ;
    void add_member(int av, int cv, int dv, int fv);
    int get_total_members() const;
    void add_action_views(int v);
    void add_comedy_views(int v);
    void add_fantasy_views(int v);
    void add_drama_views(int v);
    int get_action_views() const;
    int get_comedy_views() const;
    int get_fantasy_views() const;
    int get_drama_views() const;
    void set_action_sum(int updated_sum);
    void set_comedy_sum(int updated_sum);
    void set_drama_sum(int updated_sum);
    void set_fantasy_sum(int updated_sum);
    int get_action_sum() const;
    int get_comedy_sum() const;
    int get_drama_sum() const;
    int get_fantasy_sum() const;
    void delete_tree(std::shared_ptr<Node<std::shared_ptr<User>, UserById>> node);  /// לשים לב שהורדנו רפרנס!
    std::shared_ptr<Node<std::shared_ptr<User>, UserById>> find_user(std::shared_ptr<Node<std::shared_ptr<User>, UserById>> root, int id);
    void reduce_sum_vip();
    void add_sum_vip();
    void reduce_total_members();
    void add_group_watch();
    int sum_of_group_views() const;
    int favorite_genre() const;
};


#endif //DATA_STRUCTURES_2_GROUP_H
