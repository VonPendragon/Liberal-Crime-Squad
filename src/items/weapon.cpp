#include <externs.h>

Weapon::Weapon(const WeaponType& seed, int number) : Item(seed, number), ammo_(0)
{ }

Weapon::Weapon(const std::string& inputXml) : Item(inputXml)
{
   CMarkup xml;
   xml.SetDoc(inputXml);
   xml.FindElem();
   xml.IntoElem();
   while(xml.FindElem())
   {
      std::string tag=xml.GetTagName();
      if(tag=="loaded_cliptype")
         loaded_cliptype_=xml.GetData();
      else if(tag=="ammo")
         ammo_=atoi(xml.GetData());
   }
}

string Weapon::showXml() const
{
   CMarkup xml;
   xml.AddElem("weapon");
   xml.IntoElem();
   addBaseValues(xml);
   xml.AddElem("loaded_cliptype",loaded_cliptype_);
   xml.AddElem("ammo",tostring(ammo_));
   return xml.GetDoc();
}

string Weapon::equip_title() const
{
   string et=get_name();
   if(ammo_>0) et+=" ("+tostring(ammo_)+")";
   return et;
}

bool Weapon::reload(Clip& clip)
{
   if(acceptable_ammo(clip)&&!clip.empty())
   {
      loaded_cliptype_=clip.get_itemtypename();
      ammo_=clip.get_ammoamount();
      clip.decrease_number(1);
      return true;
   }
   else return false;
}

/*bool Weapon::take_from(Item& i, int amount)
{
   int numtaken=0;
   if(i.is_weapon()&&is_same_type(i))
   {
      Weapon& w=static_cast<Weapon&>(i); //cast -XML
      if((loaded_cliptype_==w.loaded_cliptype_&&ammo_==w.ammo_)||(ammo_==0&&w.ammo_==0))
      {
         numtaken=MIN(amount,w.get_number());
         increase_number(numtaken);
         w.set_number(w.get_number-numtaken);
      }
   }
   return numtaken;
}*/

Weapon* Weapon::split(int number)
{
   if(number>number_) number=number_;
   Weapon* newi=clone();
   newi->number_=number;
   number_-=number;
   return newi;
}

bool Weapon::merge(Item& i)
{
   if(i.is_weapon()&&is_same_type(i))
   {
      Weapon& w=static_cast<Weapon&>(i); //cast -XML
      if((loaded_cliptype_==w.loaded_cliptype_&&ammo_==w.ammo_)||(ammo_==0&&w.ammo_==0))
      {
         increase_number(w.get_number());
         w.set_number(0);
         return true;
      }
   }
   return false;
}

bool Weapon::sort_compare_special(Item* other) const
{
   if(other)
   {
      int thisi=getweapontype(itemtypename());
      int otheri=getweapontype(other->get_itemtypename());
      if(thisi<otheri||otheri==-1) return false;
      else if(thisi>otheri&&otheri!=-1) return true;
      else if(other->is_weapon())
      {
         Weapon* w=static_cast<Weapon*>(other); //cast -XML
         return ammo_<w->ammo_;
      }
      else return false;
   }
   else return false;
}


const attackst* Weapon::get_attack(bool force_ranged, bool force_melee, bool force_no_reload) const
{
   const vector<attackst*>& attacks=weapontype[getweapontype(itemtypename())]->get_attacks();
   for(int i=0;i<len(attacks);i++)
   {
      if(force_ranged&&!attacks[i]->ranged) continue;
      if(force_melee&&attacks[i]->ranged) continue;
      if(force_no_reload&&attacks[i]->uses_ammo&&!ammo_) continue;
      if(attacks[i]->uses_ammo&&attacks[i]->ammotype!=loaded_cliptype_&&ammo_) continue;
      return attacks[i];
   }
   return NULL;
}

bool Weapon::acceptable_ammo(const Item& c) const
{ return c.is_clip()&&weapontype[getweapontype(itemtypename())]->acceptable_ammo(c.get_itemtypename()); }

const string& Weapon::get_name() const
{ return weapontype[getweapontype(itemtypename())]->get_name(); }

const string& Weapon::get_name(unsigned subtype) const
{ return weapontype[getweapontype(itemtypename())]->get_name(subtype); }

const string& Weapon::get_shortname(unsigned subtype) const
{ return weapontype[getweapontype(itemtypename())]->get_shortname(subtype); }

long Weapon::get_fencevalue() const
{ return weapontype[getweapontype(itemtypename())]->get_fencevalue(); }

bool Weapon::can_take_hostages() const
{ return weapontype[getweapontype(itemtypename())]->can_take_hostages(); }

bool Weapon::is_threatening() const
{ return weapontype[getweapontype(itemtypename())]->is_threatening(); }

bool Weapon::can_threaten_hostages() const
{ return weapontype[getweapontype(itemtypename())]->can_threaten_hostages(); }

bool Weapon::protects_against_kidnapping() const
{ return weapontype[getweapontype(itemtypename())]->protects_against_kidnapping(); }

bool Weapon::has_musical_attack() const
{ return weapontype[getweapontype(itemtypename())]->has_musical_attack(); }

bool Weapon::is_instrument() const
{ return weapontype[getweapontype(itemtypename())]->is_instrument(); }

int Weapon::get_legality() const
{ return weapontype[getweapontype(itemtypename())]->get_legality(); }

float Weapon::get_bashstrengthmod() const
{ return weapontype[getweapontype(itemtypename())]->get_bashstrengthmod(); }

bool Weapon::is_suspicious() const
{ return weapontype[getweapontype(itemtypename())]->is_suspicious(); }

int Weapon::get_size() const
{ return weapontype[getweapontype(itemtypename())]->get_size(); }

bool Weapon::can_graffiti() const
{ return weapontype[getweapontype(itemtypename())]->can_graffiti(); }

bool Weapon::uses_ammo() const
{ return weapontype[getweapontype(itemtypename())]->uses_ammo(); }

bool Weapon::acceptable_ammo(const Clip& c) const
{ return weapontype[getweapontype(itemtypename())]->acceptable_ammo(c.get_itemtypename()); }

bool Weapon::acceptable_ammo(const ClipType& c) const
{ return weapontype[getweapontype(itemtypename())]->acceptable_ammo(c); }

bool Weapon::is_ranged() const
{ return weapontype[getweapontype(itemtypename())]->is_ranged(); }

bool Weapon::is_throwable() const
{ return weapontype[getweapontype(itemtypename())]->is_throwable(); }

bool Weapon::auto_breaks_locks() const
{ return weapontype[getweapontype(itemtypename())]->auto_breaks_locks(); }

bool Weapon::is_legal() const
{ return weapontype[getweapontype(itemtypename())]->is_legal(); }
