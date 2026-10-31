#include<stdio.h>
#include "populate.h"
#include "contact.h"
#include "file.h"



void populateAddressBook(AddressBook* addressBook)
{
    loadContactsFromFile(addressBook);
}