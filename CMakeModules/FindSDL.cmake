FIND_PATH(SDL_INCLUDE_DIR SDL)

FIND_LIBRARY(SDL_LIBRARY NAMES SDL)
#IF(NOT SDL_LIBRARY)
#    MESSAGE(SEND_ERROR "SDL not found")
#ENDIF(NOT SDL_LIBRARY)

IF(SDL_INCLUDE_DIR AND SDL_LIBRARY)
   SET(SDL_FOUND TRUE)
ENDIF(SDL_INCLUDE_DIR AND SDL_LIBRARY)

IF(SDL_FOUND)
   IF (NOT SDL_FIND_QUIETLY)
      MESSAGE(STATUS "Found SDL: ${SDL_LIBRARY}")
   ENDIF (NOT SDL_FIND_QUIETLY)
ELSE(SDL_FOUND)
   IF (SDL_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find SDL")
   ENDIF (SDL_FIND_REQUIRED)
ENDIF(SDL_FOUND)
