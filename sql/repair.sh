# To repair crashed mysql tables

myisamchk -r /var/db/mysql/netactuator/storage_mass.MYI
myisamchk -r /var/db/mysql/netactuator/*.MYI

# In dir /var/lib/db/mysql or /var/lib/mysql
