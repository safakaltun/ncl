      SUBROUTINE CSSTRI (N,X,Y,Z, NT,NTRI, IWK,WK,IER)
C
      INTEGER N, NT, NTRI(3,*), IWK(*), IER
      REAL    X(N), Y(N), Z(N), WK(*)
C
C  Parameters for random number usage.
C
      INTEGER CSJRAND
      PARAMETER (EPSILON=0.00001, IRMAX=32767)
      DATA IX,IY,IZ/1,2,3/
      
C
C***********************************************************
C
C                              Simplified STRIPACK interface
C                                                 Fred Clare
C                                                       NCAR
C                                                   03/04/99
C
C   This subroutine provides a simplified interface to
C STRIPACK triangulation, at the expense of requiring a
C little extra storage.
C
C On input:
C
C       N = Number of nodes in the triangulation.  N .GE. 3.
C
C       X,Y,Z = Arrays of length N containing the Cartesian
C               coordinates of distinct nodes.  (X(K),Y(K),
C               Z(K)) is referred to as node K, and K is re-
C               ferred to as a nodal index.  It is required
C               that X(K)**2 + Y(K)**2 + Z(K)**2 = 1 for all
C               K.  The first three nodes must not be col-
C               linear (lie on a common great circle).
C
C       NTRI =  A two-dimensional integer array dimensioned 
C               for 3 x NT where NT is the number of triangles
C               in the triangulation (NT is at most 2*N). 
C
C       IWK  =  An integer workspace of length 27*N.
C
C       WK   =  A real workspace of length 4*N.
C
C On output:
C
C       NT   = Number of triangles in the triangulation unless
C              IER .NE. 0, in which case NT = 0.  NT = 2N-NB-2
C              if NB .GE. 3 or 2N-4 if NB = 0, where NB is the
C              number of boundary nodes.
C
C       NTRI = The nodes for the triangles in the triangulation.
C              The nodes for the Jth triangle are NTRI(1,J),
C              NTRI(2,J) and NTRI(3,J) where node I references
C              the coordinate (X(I),Y(I),Z(I)).
C
C       IER = Error indicator:
C                =  0 - no error.
C                =  1 - invalid number of input points (must be 
C                       greater than 3).
C                =  4 - first three nodes are collinear.
C                =  6 - internal algorithm error - please report this.
C                = 10 - insufficient space for the triangulation 
C                       (must be >= number of boundary nodes minus 2).
C                = 11 - degenerate triangle (two vertices lie on 
C                       same geodesic).
C                = -L - coordinates L and M coincide for some 
C                       M  > L >= 1 (coordinate numbering 
C                       starting at 1).
C
C Modules required by CSSTRI:  CSTRMESH, CSTRLIST
C
C***********************************************************
C
C  Introduce a random perturbation in the 5th decimal place
C  to avoid duplicate input points.  The original input points
C  are copied into the real workspace so that they will not be
C  tampered with.
C
      DO 300 I=1,N
        WK(  N+I) = X(I) +
     +       EPSILON*(0.5 - REAL(CSJRAND(IRMAX,IX,IY,IZ))/REAL(IRMAX))
        WK(2*N+I) = Y(I) +
     +       EPSILON*(0.5 - REAL(CSJRAND(IRMAX,IX,IY,IZ))/REAL(IRMAX))
        WK(3*N+I) = Z(I) +
     +       EPSILON*(0.5 - REAL(CSJRAND(IRMAX,IX,IY,IZ))/REAL(IRMAX))
C
C  Renormalize the vector so that it is still a unit vector.
C
        UN =    WK(  N+I)**2 
        UN = UN+WK(2*N+I)**2 
        UN = UN+WK(3*N+I)**2 
        UN = SQRT(UN)
        WK(  N+I) = 0.99999*WK(  N+I)/UN
        WK(2*N+I) = 0.99999*WK(2*N+I)/UN
        WK(3*N+I) = 0.99999*WK(3*N+I)/UN
  300 CONTINUE
C
C  Triangulate.
C
      CALL CSTRMESH(N,WK(N+1),WK(2*N+1),WK(3*N+1),
     +              IWK(1),IWK(6*N+1),IWK(12*N+1),LNEW,
     +              IWK(13*N+1),IWK(14*N+1),WK(1),IER)
      IF (IER .EQ. 0) THEN
        GO TO 210
      ELSE IF (IER .EQ. -1) THEN
        IER = 1
        GO TO 200
      ELSE IF (IER .EQ. -2) THEN
        IER = 4
        GO TO 200
      ELSE IF (IER .EQ. -3) THEN
        IER = 6
        GO TO 200
      ELSE IF (IER .GT. 0) THEN
        IER = -IER
        GO TO 200
      ELSE
        IER = 6
        GO TO 200
      ENDIF
C
  210 CONTINUE
C
C  Determine the triangle nodes.
C
      CALL CSTRLIST(N,IWK(1),IWK(6*N+1),IWK(12*N+1),
     +              6,NT,IWK(15*N+1),IER)
      IF (IER .NE. 0) THEN
        NT = 0
        IER = 6
        GO TO 200
      ENDIF
C
C  Copy off the triangle nodes and return.
C
      DO 10 J=1,NT
        NTRI(1,J) = IWK(15*N+6*(J-1)+1) 
        NTRI(2,J) = IWK(15*N+6*(J-1)+2) 
        NTRI(3,J) = IWK(15*N+6*(J-1)+3) 
   10 CONTINUE
C
      RETURN
C
  200 CONTINUE
      CALL CSSERR('CSSTRI',IER)
      NT = 0
      RETURN
C
      END
