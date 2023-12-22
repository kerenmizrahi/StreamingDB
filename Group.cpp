//
// Created by keren on 09/05/2023.
//
#include "Group.h"

int Group::get_id() const {
    return groupId;
}

bool Group::is_vip() const {
    if (sumVip > 0)
        return true;
    return false;
}

AVLTree<std::shared_ptr<User>, UserById>& Group::get_members() {
    return members;
}

int Group::get_total_members() const{
    return totalMembers;
}

void Group::add_member(int av, int cv, int dv, int fv){
    action_sum += av;
    comedy_sum += cv;
    drama_sum += dv;
    fantasy_sum += fv;

    totalMembers++;
}

void Group::add_action_views(int v) {
    actionViews += v;  }

void Group::add_comedy_views(int v) {
    comedyViews += v;  }

void Group::add_fantasy_views(int v) {
    fantasyViews += v;}

void Group::add_drama_views(int v) {
    dramaViews += v;  }

int Group::get_action_views() const{
    return actionViews; }

int Group::get_comedy_views() const{
    return comedyViews; }

int Group::get_fantasy_views() const {
    return fantasyViews; }

int Group::get_drama_views() const {
    return dramaViews; }

void Group::delete_tree(std::shared_ptr<Node<std::shared_ptr<User>, UserById>> node) {
    if (node) {
        delete_tree(node->left);
        delete_tree(node->right);
        node->right.reset();
        node->left.reset();
        if (node->value->get_group())
            node->value->views_after_removal(this->actionViews, this->comedyViews, this->dramaViews, this->fantasyViews);
        node->value->set_group(nullptr);
        //node->value.reset(); /// לשים לב שאולי לא צריך!
        node.reset();
    }
}


std::shared_ptr<Node<std::shared_ptr<User>, UserById>> Group::find_user(std::shared_ptr<Node<std::shared_ptr<User>, UserById>> root, int id){
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

int Group::get_action_sum() const{
    return action_sum;
}
int Group::get_comedy_sum() const{
    return comedy_sum;
}
int Group::get_drama_sum() const{
    return drama_sum;
}
int Group::get_fantasy_sum() const{
    return fantasy_sum;
}

void Group::reduce_sum_vip() {
    sumVip --;
}

void Group::add_sum_vip(){
    sumVip++;
}

void Group::reduce_total_members() {
    totalMembers --;
}

void Group::set_action_sum(int updated_sum){
    action_sum = updated_sum;
}
void Group::set_comedy_sum(int updated_sum){
    comedy_sum = updated_sum;
}
void Group::set_drama_sum(int updated_sum){
    drama_sum = updated_sum;
}
void Group::set_fantasy_sum(int updated_sum){
    fantasy_sum = updated_sum;
}

int Group::sum_of_group_views() const{
    return (actionViews + comedyViews + dramaViews + fantasyViews);
}

int Group::favorite_genre() const{
    int max_sum = comedy_sum;
    int max_genre = 0;

    if (drama_sum > comedy_sum) {
        max_sum = drama_sum;
        max_genre = 1;
    }

    if (action_sum > max_sum){
        max_sum = action_sum;
        max_genre = 2;
    }

    if (fantasy_sum > max_sum){
        max_sum = fantasy_sum;
        max_genre = 3;
    }

    return max_genre;
}