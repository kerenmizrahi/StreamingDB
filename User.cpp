//
// Created by keren on 09/05/2023.
//

#include "User.h"

bool User::is_vip() const {
    return isVip;
}

void User::init_fix(int av, int cv, int dv, int fv) {
    action_fix = av;
    comedy_fix = cv;
    drama_fix = dv;
    fantasy_fix = fv;
}

void User::views_after_removal(int av, int cv, int dv, int fv){
    actionViews += (av - action_fix);
    comedyViews += (cv - comedy_fix);
    dramaViews += (dv - drama_fix);
    fantasyViews += (fv - fantasy_fix);
    init_fix(0,0,0,0);
}

void User::add_action_views(int v) {
    actionViews += v;  }

void User::add_comedy_views(int v) {
    comedyViews += v;  }

void User::add_fantasy_views(int v) {
    fantasyViews += v;}

void User::add_drama_views(int v) {
    dramaViews += v;  }

int User::get_action_views() const{
    return actionViews; }

int User::get_comedy_views() const{
    return comedyViews; }

int User::get_fantasy_views() const {
    return fantasyViews; }

int User::get_drama_views() const {
    return dramaViews; }

void User::set_group(std::shared_ptr<Group> g) {
    group = g;
}

std::shared_ptr<Group> User::get_group() const {
    return group; }

int User::get_id() const {
    return userId;
}

int User::sum_of_fix_views() const{
    return comedyViews - comedy_fix + actionViews - action_fix + dramaViews - drama_fix + fantasyViews - fantasy_fix;
}

int User::sum_of_views() const{
    return (comedyViews + actionViews + dramaViews + fantasyViews);
}

int User::get_drama_fix() const{
    return drama_fix;
}

int User::get_action_fix() const{
    return action_fix;
}

int User::get_comedy_fix() const{
    return comedy_fix;
}

int User::get_fantasy_fix() const{
    return fantasy_fix;
}