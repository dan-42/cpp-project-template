


#include <pugixml.hpp>

#include <pre/xml/to_xml.hpp>
#include <pre/xml/from_xml.hpp>

#include <iostream>

  struct version_1{};

  BOOST_FUSION_ADAPT_STRUCT(version_1
  );

 struct customer {
    version_1 version;
   std::string name;
   size_t money_spent;
   std::vector<std::string> interests;
   std::map<std::string, double> languages_skills;
 };

 BOOST_FUSION_ADAPT_STRUCT(customer,
     version,
   name,
   money_spent,
   interests,
   languages_skills
 );




int main(int argc, char **argv)
{


   customer my_customer{
     {},
     "Mr. Dupond",
     1000,
     {"sport articles", "food", "tools"},
     {{"de", 3.14}, {"en", 42}, {"fr", 0.0}, {"c++", 1337}, }
   };

   {
     pugi::xml_document doc{};
     pre::xml::to_xml(doc, "my_customer", my_customer);
     doc.save(std::cout);
   }

   {

    const auto xml_string = pre::xml::to_xml( "my_customer", my_customer);
    auto my_customer2 = pre::xml::from_xml<customer>(xml_string);
    std::cout << my_customer2.name << std::endl;
    std::cout << my_customer2.money_spent << std::endl;
    for(const auto&  i : my_customer2.interests)
    {
      std::cout << i << std::endl;
    }

    for(const auto& pair : my_customer2.languages_skills)
    {
      std::cout << pair.first << " " << pair.second << std::endl;
    }


   }


  return 0;
}


