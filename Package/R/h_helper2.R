h_helper2<-function(x,breaks) {
  # if a break computing function name is passed
  if(is.character(breaks))
    nbreaks<-do.call(paste("nclass",breaks,sep=".",collapse=""),list(x))
  # if breaks is numeric
  if(is.numeric(breaks)) {
    # if just the number of breaks is passed
    if(length(breaks) == 1) {
      nbreaks<-breaks
    }
    # otherwise assume that breaks specifies the breakpoints
    else return(breaks)
  }
  breakinc<-diff(range(x))/nbreaks
  breaks<-c(min(x),rep(breakinc,nbreaks))
  breaks<-cumsum(breaks)
  return(breaks)
}
