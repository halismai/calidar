function fig2eps(fname,hdle)
  % function fig2eps(fname,hdle) 
  % 
  % Writes the figure encapsulated post script EPS file
  % 
  % INPUT 
  %    fname    output filename [.eps] will be appended by defulat if fname does
  %    not have it 
  %    hdle     the figure handle you want to eps
  % 
  % Hatem Alismail <halismai@cs.cmu.edu>
  % Last modified: Fri 15 Feb 2013 03:36:40 PM EST
  %

  if nargin < 2
    hdle=gcf; % default to current figure
  end

  [pathstr,name,ext] = fileparts(fname);
  if ~strcmp(ext,'.eps')
    fname = [fname '.eps'];
  end

  set(hdle,'PaperPositionMode','auto');
  print('-depsc2', '-r300', fname);
  fixPSlinestyle(fname,fname); % this function is in externals

end % fig2eps

