This is a command which prints the Tox ID for the current user, or for a specified user, to the console. It may be useful for scripting interactions with Tox communication systems, or making IDs visible on a network via Avahi services so that they are discoverable.

To compile this you will first need to install toxcore (https://github.com/irungentoo/toxcore)

    make
    sudo make install

To print your own ID:

    toxid

or for a given user:

    toxid -u [username]

To advertise your Tox ID via an Avahi service

    sudo toxavahi [username]

Then on another system on the same network to discover Tox IDs:

    lstox
