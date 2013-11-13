function c = reflectance2real(r, range, max_ref)
  %function c = reflectance2real(r, range, max_ref)

  if nargin < 3, max_ref = 7500.0; end % arbitrary selected value here

  r = double(r); 

  c = min(r.*sqrt(range)/sqrt(2), max_ref) / max_ref;
  c = (c - min(c(:))) / (max(c(:))-min(c(:)));

end
